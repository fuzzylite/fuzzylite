// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

/*
 * RuleBlock.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/rule/RuleBlock.h"

#include "fl/rule/Rule.h"

#include "fl/norm/TNorm.h"
#include "fl/norm/SNorm.h"

#include "fl/imex/FllExporter.h"

#include <sstream>

namespace fl {

    RuleBlock::RuleBlock(const std::string& name)
    : _name(name), _conjunction(NULL), _disjunction(NULL), _activation(NULL), _enabled(true) {
    }

    RuleBlock::~RuleBlock() {
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            delete _rules.at(i);
        }
        if (_conjunction) delete _conjunction;
        if (_disjunction) delete _disjunction;
        if (_activation) delete _activation;
    }

    void RuleBlock::activate() {
        FL_DBG("===================");
        FL_DBG("ACTIVATING RULEBLOCK " << _name);
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            Rule* rule = _rules.at(i);
            if (rule->isLoaded()) {
                scalar activationDegree = rule->activationDegree(_conjunction, _disjunction);
                FL_DBG(rule->toString() << " [activationDegree=" << activationDegree << "]");
                if (Op::isGt(activationDegree, 0.0)) {
                    rule->activate(activationDegree, _activation);
                }
            } else {
                FL_DBG("Rule not loaded: " << rule->toString());
            }
        }
    }

    void RuleBlock::unloadRules() const {
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            Rule* rule = _rules.at(i);
            rule->unload();
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
            fl::Exception exception("[ruleblock error] the following "
                    "rules could not be loaded:\n" + exceptions.str(), FL_AT);
            throw exception;
        }
    }

    void RuleBlock::setName(std::string name) {
        this->_name = name;
    }

    std::string RuleBlock::getName() const {
        return this->_name;
    }

    void RuleBlock::setConjunction(TNorm* tnorm) {
        this->_conjunction = tnorm;
    }

    TNorm* RuleBlock::getConjunction() const {
        return this->_conjunction;
    }

    void RuleBlock::setDisjunction(SNorm* snorm) {
        this->_disjunction = snorm;
    }

    SNorm* RuleBlock::getDisjunction() const {
        return this->_disjunction;
    }

    void RuleBlock::setActivation(TNorm* activation) {
        this->_activation = activation;
    }

    TNorm* RuleBlock::getActivation() const {
        return this->_activation;
    }

    void RuleBlock::setEnabled(bool enabled) {
        this->_enabled = enabled;
    }

    bool RuleBlock::isEnabled() const {
        return this->_enabled;
    }

    void RuleBlock::clear() {
        setName("");
        setEnabled(true);
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            _rules.at(i)->unload();
            delete _rules.at(i);
        }
        _rules.clear();
        if (_activation) {
            delete _activation;
            setActivation(NULL);
        }
        if (_conjunction) {
            delete _conjunction;
            setConjunction(NULL);
        }
        if (_disjunction) {
            delete _disjunction;
            setDisjunction(NULL);
        }
    }

    std::string RuleBlock::toString() const {
        return FllExporter("", "; ").toString(this);
    }

    /**
     * Operations for std::vector _rules
     */
    void RuleBlock::addRule(Rule* rule) {
        this->_rules.push_back(rule);
    }

    void RuleBlock::insertRule(Rule* rule, int index) {
        this->_rules.insert(this->_rules.begin() + index, rule);
    }

    Rule* RuleBlock::getRule(int index) const {
        return this->_rules.at(index);
    }

    Rule* RuleBlock::removeRule(int index) {
        Rule* result = this->_rules.at(index);
        this->_rules.erase(this->_rules.begin() + index);
        return result;
    }

    int RuleBlock::numberOfRules() const {
        return this->_rules.size();
    }

    const std::vector<Rule*>& RuleBlock::rules() const {
        return this->_rules;
    }

    void RuleBlock::setRules(const std::vector<Rule*>& rules) {
        this->_rules = rules;
    }



}
