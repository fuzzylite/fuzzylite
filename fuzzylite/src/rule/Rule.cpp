/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/rule/Rule.h"

#include "fl/Exception.h"
#include "fl/hedge/Hedge.h"
#include "fl/imex/FllExporter.h"
#include "fl/norm/Norm.h"
#include "fl/rule/Antecedent.h"
#include "fl/rule/Consequent.h"
#include "fl/Operation.h"

#include <sstream>
#include <vector>

namespace fl {

    Rule::Rule(const std::string& text, scalar weight)
    : _text(text), _weight(weight), _antecedent(new Antecedent), _consequent(new Consequent),
    _activationDegree(0.0), _activated(false) {
    }

    Rule::Rule(const Rule& other) : _text(other._text), _weight(other._weight),
    _antecedent(new Antecedent), _consequent(new Consequent),
    _activationDegree(other._activationDegree), _activated(other._activated) {
    }

    Rule& Rule::operator=(const Rule& other) {
        if (this != &other) {
            _text = other._text;
            _weight = other._weight;
            _antecedent.reset(new Antecedent);
            _consequent.reset(new Consequent);
            _activationDegree = other._activationDegree;
            _activated = other._activated;
        }
        return *this;
    }

    Rule::~Rule() {
        unload();
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

    void Rule::setActivated(bool activated) {
        this->_activated = activated;
    }

    bool Rule::isActivated() const {
        return this->_activated;
    }

    void Rule::setActivationDegree(scalar activationDegree) {
        this->_activationDegree = activationDegree;
    }

    scalar Rule::getActivationDegree() const {
        return this->_activationDegree;
    }

    scalar Rule::computeActivationDegree(const TNorm* conjunction, const SNorm* disjunction) const {
        if (not isLoaded()) {
            throw fl::Exception("[rule error] the following rule is not loaded: " + getText(), FL_AT);
        }
        return getWeight() * getAntecedent()->activationDegree(conjunction, disjunction);
    }

    void Rule::activate(scalar activationDegree, const TNorm* implication) {
        FL_DBG("[activating] " << toString());
        if (not isLoaded()) {
            throw fl::Exception("[rule error] the following rule is not loaded: " + getText(), FL_AT);
        }
        if (Op::isGt(activationDegree, 0.0)) {
            FL_DBG("[degree=" << Op::str(activationDegree) << "] " << toString());
            setActivationDegree(activationDegree);
            getConsequent()->modify(activationDegree, implication);
        }
        setActivated(true);
    }

    void Rule::deactivate() {
        setActivated(false);
        setActivationDegree(0.0);
        FL_DBG("[deactivated] " << toString());
    }

    bool Rule::isLoaded() const {
        if (getAntecedent() and getConsequent()) {
            return getAntecedent()->isLoaded() and getConsequent()->isLoaded();
        }
        return false;
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
        setText(rule);
        setActivated(false);
        setActivationDegree(0.0);
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
                            throw fl::Exception(ex.str(), FL_AT);
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
                            weight = fl::Op::toScalar(token);
                            state = S_END;
                        } catch (fl::Exception& e) {
                            std::ostringstream ex;
                            ex << "[syntax error] expected a numeric value as the weight of the rule: "
                                    << rule;
                            e.append(ex.str(), FL_AT);
                            throw e;
                        }
                        break;
                    case S_END:
                        std::ostringstream ex;
                        ex << "[syntax error] unexpected token <" << token << "> at the end of rule";
                        throw fl::Exception(ex.str(), FL_AT);
                }
            }
            if (state == S_NONE) {
                std::ostringstream ex;
                ex << "[syntax error] " << (rule.empty() ? "empty rule" : "ignored rule: " + rule);
                throw fl::Exception(ex.str(), FL_AT);
            } else if (state == S_IF) {
                std::ostringstream ex;
                ex << "[syntax error] keyword <" << Rule::thenKeyword() << "> not found in rule: " << rule;
                throw fl::Exception(ex.str(), FL_AT);
            } else if (state == S_WITH) {
                std::ostringstream ex;
                ex << "[syntax error] expected a numeric value as the weight of the rule: " << rule;
                throw fl::Exception(ex.str(), FL_AT);
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
