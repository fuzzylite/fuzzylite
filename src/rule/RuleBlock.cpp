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

#include "fl/rule/RuleBlock.h"

#include "fl/activation/General.h"
#include "fl/imex/FllExporter.h"
#include "fl/norm/TNorm.h"
#include "fl/norm/SNorm.h"
#include "fl/rule/Rule.h"
#include "fl/Operation.h"

namespace fl {

    RuleBlock::RuleBlock(const std::string& name)
    : _enabled(true), _name(name), _description("") { }

    RuleBlock::RuleBlock(const RuleBlock& other) : _enabled(true), _name(other._name),
    _description(other._description) {
        copyFrom(other);
    }

    RuleBlock& RuleBlock::operator=(const RuleBlock& other) {
        if (this != &other) {
            for (std::size_t i = 0; i < _rules.size(); ++i) {
                delete _rules.at(i);
            }
            _rules.clear();
            _conjunction.reset(fl::null);
            _disjunction.reset(fl::null);
            _implication.reset(fl::null);
            _activation.reset(fl::null);

            copyFrom(other);
        }
        return *this;
    }

    void RuleBlock::copyFrom(const RuleBlock& source) {
        _enabled = source._enabled;
        _name = source._name;
        _description = source._description;
        if (source._conjunction.get()) _conjunction.reset(source._conjunction->clone());
        if (source._disjunction.get()) _disjunction.reset(source._disjunction->clone());
        if (source._implication.get()) _implication.reset(source._implication->clone());
        if (source._activation.get()) _activation.reset(source._activation->clone());
        for (std::size_t i = 0; i < source._rules.size(); ++i) {
            _rules.push_back(source._rules.at(i)->clone());
        }
    }

    RuleBlock::~RuleBlock() {
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            delete _rules.at(i);
        }
        _rules.clear();
    }

    Complexity RuleBlock::complexity() const {
        Complexity result;
        result.comparison(1);
        if (_activation.get()) {
            result += _activation->complexity(this);
        } else {
            for (std::size_t i = 0; i < _rules.size(); ++i) {
                result += _rules.at(i)->complexity(
                        _conjunction.get(), _disjunction.get(), _implication.get());
            }
        }
        return result;
    }

    void RuleBlock::activate() {
        if (not _activation.get()) {
            _activation.reset(new General);
        }
        _activation->activate(this);
    }

    void RuleBlock::unloadRules() const {
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            _rules.at(i)->unload();
        }
    }

    void RuleBlock::loadRules(const Engine* engine) {
        std::ostringstream exceptions;
        bool throwException = false;
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            Rule* rule = _rules.at(i);
            if (rule->isLoaded()) {
                rule->unload();
            }
            try {
                rule->load(engine);
            } catch (std::exception& ex) {
                throwException = true;
                exceptions << ex.what() << "\n";
            }
        }
        if (throwException) {
            Exception exception("[ruleblock error] the following "
                    "rules could not be loaded:\n" + exceptions.str(), FL_AT);
            throw exception;
        }
    }

    void RuleBlock::reloadRules(const Engine* engine) {
        unloadRules();
        loadRules(engine);
    }

    void RuleBlock::setName(std::string name) {
        this->_name = name;
    }

    std::string RuleBlock::getName() const {
        return this->_name;
    }

    void RuleBlock::setDescription(const std::string& description) {
        this->_description = description;
    }

    std::string RuleBlock::getDescription() const {
        return this->_description;
    }

    void RuleBlock::setConjunction(TNorm* tnorm) {
        this->_conjunction.reset(tnorm);
    }

    TNorm* RuleBlock::getConjunction() const {
        return this->_conjunction.get();
    }

    void RuleBlock::setDisjunction(SNorm* snorm) {
        this->_disjunction.reset(snorm);
    }

    SNorm* RuleBlock::getDisjunction() const {
        return this->_disjunction.get();
    }

    void RuleBlock::setImplication(TNorm* implication) {
        this->_implication.reset(implication);
    }

    TNorm* RuleBlock::getImplication() const {
        return this->_implication.get();
    }

    void RuleBlock::setActivation(Activation* activation) {
        this->_activation.reset(activation);
    }

    Activation* RuleBlock::getActivation() const {
        return this->_activation.get();
    }

    void RuleBlock::setEnabled(bool enabled) {
        this->_enabled = enabled;
    }

    bool RuleBlock::isEnabled() const {
        return this->_enabled;
    }

    std::string RuleBlock::toString() const {
        return FllExporter().toString(this);
    }

    /**
     * Operations for std::vector _rules
     */
    void RuleBlock::addRule(Rule* rule) {
        _rules.push_back(rule);
    }

    void RuleBlock::insertRule(Rule* rule, std::size_t index) {
        _rules.insert(_rules.begin() + index, rule);
    }

    Rule* RuleBlock::getRule(std::size_t index) const {
        return _rules.at(index);
    }

    Rule* RuleBlock::removeRule(std::size_t index) {
        Rule* result = _rules.at(index);
        _rules.erase(_rules.begin() + index);
        return result;
    }

    std::size_t RuleBlock::numberOfRules() const {
        return _rules.size();
    }

    const std::vector<Rule*>& RuleBlock::rules() const {
        return this->_rules;
    }

    void RuleBlock::setRules(const std::vector<Rule*>& rules) {
        this->_rules = rules;
    }

    std::vector<Rule*>& RuleBlock::rules() {
        return this->_rules;
    }

    RuleBlock* RuleBlock::clone() const {
        return new RuleBlock(*this);
    }

}
