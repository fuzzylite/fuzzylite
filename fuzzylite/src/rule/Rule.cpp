/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/rule/Rule.h"

#include "fl/Exception.h"
#include "fl/imex/FllExporter.h"
#include "fl/norm/Norm.h"
#include "fl/Operation.h"

namespace fl {

    Rule::Rule(const std::string& text, scalar weight)
    : _enabled(true), _text(text), _weight(weight), _activationDegree(0.0), _triggered(false),
    _antecedent(new Antecedent), _consequent(new Consequent) { }

    Rule::Rule(const Rule& other) : _enabled(other._enabled), _text(other._text),
    _weight(other._weight), _activationDegree(other._activationDegree), _triggered(false),
    _antecedent(new Antecedent), _consequent(new Consequent) { }

    Rule& Rule::operator=(const Rule& other) {
        if (this != &other) {
            _enabled = other._enabled;
            _text = other._text;
            _weight = other._weight;
            _activationDegree = other._activationDegree;
            _triggered = other._triggered;
            _antecedent.reset(new Antecedent);
            _consequent.reset(new Consequent);
        }
        return *this;
    }

    Rule::~Rule() {
        if (_antecedent.get()) _antecedent->unload();
        if (_consequent.get()) _consequent->unload();
    }

    void Rule::setText(const std::string& text) {
        this->_text = text;
    }

    std::string Rule::getText() const {
        return this->_text;
    }

    void Rule::setWeight(scalar weight) {
        this->_weight = weight;
    }

    scalar Rule::getWeight() const {
        return this->_weight;
    }

    void Rule::setAntecedent(Antecedent* antecedent) {
        this->_antecedent.reset(antecedent);
    }

    Antecedent* Rule::getAntecedent() const {
        return this->_antecedent.get();
    }

    void Rule::setConsequent(Consequent* consequent) {
        this->_consequent.reset(consequent);
    }

    Consequent* Rule::getConsequent() const {
        return this->_consequent.get();
    }

    void Rule::setEnabled(bool active) {
        this->_enabled = active;
    }

    bool Rule::isEnabled() const {
        return this->_enabled;
    }

    void Rule::setActivationDegree(scalar activationDegree) {
        this->_activationDegree = activationDegree;
    }

    scalar Rule::getActivationDegree() const {
        return this->_activationDegree;
    }

    void Rule::deactivate() {
        _activationDegree = 0.0;
        _triggered = false;
    }

    scalar Rule::activateWith(const TNorm* conjunction, const SNorm* disjunction) {
        if (not isLoaded()) {
            throw Exception("[rule error] the following rule is not loaded: " + getText(), FL_AT);
        }
        _activationDegree = _weight * _antecedent->activationDegree(conjunction, disjunction);
        return _activationDegree;
    }

    void Rule::trigger(const TNorm* implication) {
        if (not isLoaded()) {
            throw Exception("[rule error] the following rule is not loaded: " + getText(), FL_AT);
        }
        if (_enabled and Op::isGt(_activationDegree, 0.0)) {
            FL_DBG("[firing with " << Op::str(_activationDegree) << "] " << toString());
            _consequent->modify(_activationDegree, implication);
            _triggered = true;
        }
    }

    bool Rule::isTriggered() const {
        return this->_triggered;
    }

    Complexity Rule::complexityOfActivation(const TNorm* conjunction, const SNorm* disjunction) const {
        Complexity result;
        result.comparison(1).arithmetic(1);
        if (isLoaded()) {
            result += _antecedent->complexity(conjunction, disjunction);
        }
        return result;
    }

    Complexity Rule::complexityOfFiring(const TNorm* implication) const {
        Complexity result;
        result.comparison(3);
        if (isLoaded()) {
            result += _consequent->complexity(implication);
        }
        return result;
    }

    Complexity Rule::complexity(const TNorm* conjunction, const SNorm* disjunction,
            const TNorm* implication) const {
        return complexityOfActivation(conjunction, disjunction)
                + complexityOfFiring(implication);
    }

    bool Rule::isLoaded() const {
        return _antecedent.get() and _consequent.get()
                and _antecedent->isLoaded() and _consequent->isLoaded();
    }

    void Rule::unload() {
        deactivate();
        if (getAntecedent()) getAntecedent()->unload();
        if (getConsequent()) getConsequent()->unload();
    }

    void Rule::load(const Engine* engine) {
        load(getText(), engine);
    }

    void Rule::load(const std::string& rule, const Engine* engine) {
        deactivate();
        setEnabled(true);
        setText(rule);
        std::istringstream tokenizer(rule.substr(0, rule.find_first_of('#')));
        std::string token;
        std::ostringstream ossAntecedent, ossConsequent;
        scalar weight = 1.0;

        enum FSM {
            S_NONE, S_IF, S_THEN, S_WITH, S_END
        };
        FSM state = S_NONE;
        try {
            while (tokenizer >> token) {

                switch (state) {
                    case S_NONE:
                        if (token == Rule::ifKeyword()) state = S_IF;
                        else {
                            std::ostringstream ex;
                            ex << "[syntax error] expected keyword <" << Rule::ifKeyword() <<
                                    ">, but found <" << token << "> in rule: " << rule;
                            throw Exception(ex.str(), FL_AT);
                        }
                        break;
                    case S_IF:
                        if (token == Rule::thenKeyword()) state = S_THEN;
                        else ossAntecedent << token << " ";
                        break;
                    case S_THEN:
                        if (token == Rule::withKeyword()) state = S_WITH;
                        else ossConsequent << token << " ";
                        break;
                    case S_WITH:
                        try {
                            weight = Op::toScalar(token);
                            state = S_END;
                        } catch (Exception& e) {
                            std::ostringstream ex;
                            ex << "[syntax error] expected a numeric value as the weight of the rule: "
                                    << rule;
                            e.append(ex.str(), FL_AT);
                            throw;
                        }
                        break;
                    case S_END:
                    {
                        std::ostringstream ex;
                        ex << "[syntax error] unexpected token <" << token << "> at the end of rule";
                        throw Exception(ex.str(), FL_AT);
                    }

                    default:
                        std::ostringstream ex;
                        ex << "[syntax error] unexpected state <" << state << ">";
                        throw Exception(ex.str(), FL_AT);
                }
            }
            if (state == S_NONE) {
                std::ostringstream ex;
                ex << "[syntax error] " << (rule.empty() ? "empty rule" : ("ignored rule: " + rule));
                throw Exception(ex.str(), FL_AT);
            } else if (state == S_IF) {
                std::ostringstream ex;
                ex << "[syntax error] keyword <" << Rule::thenKeyword() << "> not found in rule: " << rule;
                throw Exception(ex.str(), FL_AT);
            } else if (state == S_WITH) {
                std::ostringstream ex;
                ex << "[syntax error] expected a numeric value as the weight of the rule: " << rule;
                throw Exception(ex.str(), FL_AT);
            }

            getAntecedent()->load(ossAntecedent.str(), engine);
            getConsequent()->load(ossConsequent.str(), engine);
            setWeight(weight);

        } catch (...) {
            unload();
            throw;
        }
    }

    std::string Rule::toString() const {
        return FllExporter().toString(this);
    }

    Rule* Rule::clone() const {
        return new Rule(*this);
    }

    Rule* Rule::parse(const std::string& rule, const Engine* engine) {
        FL_unique_ptr<Rule> result(new Rule);
        result->load(rule, engine);
        return result.release();
    }

}
