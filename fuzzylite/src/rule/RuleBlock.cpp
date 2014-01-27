/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
    }

    void RuleBlock::activate() {
        FL_DBG("===================");
        FL_DBG("ACTIVATING RULEBLOCK " << _name);
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            scalar activationDegree = _rules.at(i)->activationDegree(_conjunction, _disjunction);
            FL_DBG(_rules.at(i)->toString() << " [activationDegree=" << activationDegree << "]");
            if (Op::isGt(activationDegree, 0.0)) {
                _rules.at(i)->activate(activationDegree, _activation);
            }
        }
    }

    void RuleBlock::setName(std::string name) {
        this->_name = name;
    }

    std::string RuleBlock::getName() const {
        return this->_name;
    }

    void RuleBlock::setConjunction(const TNorm* tnorm) {
        if (this->_conjunction) delete this->_conjunction;
        this->_conjunction = tnorm;
    }

    const TNorm* RuleBlock::getConjunction() const {
        return this->_conjunction;
    }

    void RuleBlock::setDisjunction(const SNorm* snorm) {
        if (this->_disjunction) delete this->_disjunction;
        this->_disjunction = snorm;
    }

    const SNorm* RuleBlock::getDisjunction() const {
        return this->_disjunction;
    }

    void RuleBlock::setActivation(const TNorm* activation) {
        if (this->_activation) delete this->_activation;
        this->_activation = activation;
    }

    const TNorm* RuleBlock::getActivation() const {
        return this->_activation;
    }

    void RuleBlock::setEnabled(bool enabled) {
        this->_enabled = enabled;
    }

    bool RuleBlock::isEnabled() const {
        return this->_enabled;
    }

    std::string RuleBlock::toString() const {
        return FllExporter("", "; ").toString(this);
    }

    /**
     * Operations for datatype _rules
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

}
