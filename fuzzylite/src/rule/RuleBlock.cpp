/*
 * RuleBlock.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "RuleBlock.h"

#include "Rule.h"

#include "../engine/Configuration.h"
#include "../engine/Operator.h"

#include <sstream>

namespace fl {

    RuleBlock::RuleBlock(const std::string& name)
            : _name(name) {
    }

    RuleBlock::~RuleBlock() {
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            delete _rules[i];
        }
    }

    void RuleBlock::configure(Configuration* config) {
        this->_tnorm = config->getTnorm();
        this->_snorm = config->getSnorm();
        this->_activation = config->getActivation();
    }

    void RuleBlock::fireRules() {
        for (std::size_t i = 0; i < _rules.size(); ++i) {
            scalar strength = _rules[i]->firingStrength(_tnorm, _snorm);
            if (Op::IsGt(strength, 0.0)) {
                _rules[i]->fire(strength, _activation);
            }
        }
    }

    void RuleBlock::setName(std::string name) {
        this->_name = name;
    }
    std::string RuleBlock::getName() const {
        return this->_name;
    }
    void RuleBlock::setTnorm(const Operator* tnorm) {
        this->_tnorm = tnorm;
    }
    const Operator* RuleBlock::getTnorm() const {
        return this->_tnorm;
    }
    void RuleBlock::setSnorm(const Operator* snorm) {
        this->_snorm = snorm;
    }
    const Operator* RuleBlock::getSnorm() const {
        return this->_snorm;
    }
    void RuleBlock::setActivation(const Operator* activation) {
        this->_activation = activation;
    }
    const Operator* RuleBlock::getActivation() const {
        return this->_activation;
    }

    std::string RuleBlock::toString() const {
        std::stringstream ss;
        ss << "name ='" << _name << "' "
                << "tnorm='" << _tnorm->name() << "' "
                << "snorm='" << _snorm->name() << "' "
                << "activation='" << _activation->name() << "' "
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
        return this->_rules[index];
    }
    Rule* RuleBlock::removeRule(int index) {
        Rule* result = this->_rules[index];
        this->_rules.erase(this->_rules.begin() + index);
        return result;
    }
    int RuleBlock::numberOfRules() const {
        return this->_rules.size();
    }
    const std::vector<Rule*>& RuleBlock::rules() const {
        return this->_rules;
    }

} /* namespace fl */
