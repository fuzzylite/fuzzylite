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

#include "fl/operator/TNorm.h"
#include "fl/operator/SNorm.h"

#include <sstream>

namespace fl {

    RuleBlock::RuleBlock(const std::string& name)
    : _name(name), _tnorm(NULL), _snorm(NULL), _activation(NULL) { }

    RuleBlock::~RuleBlock() {
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            delete _rules.at(i);
        }
    }

    void RuleBlock::fireRules() {
        FL_DBG("===================");
        FL_DBG("FIRING RULEBLOCK " << _name);
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            scalar strength = _rules.at(i)->firingStrength(_tnorm, _snorm);
            FL_DBG(_rules.at(i)->toString() << " [strength=" << strength << "]");
            if (Op::isGt(strength, 0.0)) {
                _rules.at(i)->fire(strength, _activation);
            }
        }
    }

    void RuleBlock::setName(std::string name) {
        this->_name = name;
    }

    std::string RuleBlock::getName() const {
        return this->_name;
    }

    void RuleBlock::setTnorm(const TNorm* tnorm) {
        if (this->_tnorm) delete this->_tnorm;
        this->_tnorm = tnorm;
    }

    const TNorm* RuleBlock::getTnorm() const {
        return this->_tnorm;
    }

    void RuleBlock::setSnorm(const SNorm* snorm) {
        if (this->_snorm) delete this->_snorm;
        this->_snorm = snorm;
    }

    const SNorm* RuleBlock::getSnorm() const {
        return this->_snorm;
    }

    void RuleBlock::setActivation(const TNorm* activation) {
        if (this->_activation) delete this->_activation;
        this->_activation = activation;
    }

    const TNorm* RuleBlock::getActivation() const {
        return this->_activation;
    }

    std::string RuleBlock::toString() const {
        std::stringstream ss;
        ss << "name ='" << _name << "' "
                << "tnorm='" << _tnorm->className() << "' "
                << "snorm='" << _snorm->className() << "' "
                << "activation='" << _activation->className() << "' "
                ;
        return ss.str();
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
