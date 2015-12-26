/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/activation/Lowest.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/rule/Antecedent.h"
#include "fl/Operation.h"

#include <queue>

namespace fl {

    Lowest::Lowest(int numberOfRules) : Activation(), _activatedRules(numberOfRules) {
    }

    Lowest::~Lowest() {

    }

    std::string Lowest::className() const {
        return "Lowest";
    }

    std::string Lowest::parameters() const {
        return Op::str(getActivatedRules());
    }

    void Lowest::configure(const std::string& parameters) {
        setActivatedRules((int) Op::toScalar(parameters));
    }

    int Lowest::getActivatedRules() const {
        return this->_activatedRules;
    }

    void Lowest::setActivatedRules(int activatedRules) {
        this->_activatedRules = activatedRules;
    }

    struct RuleDegreeComparatorAscending {

        bool operator()(const Rule* a, const Rule* b) {
            return a->getActivationDegree() > b->getActivationDegree();
        }
    };

    void Lowest::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        std::priority_queue<Rule*, std::vector<Rule*>,
                RuleDegreeComparatorAscending> rulesToActivate;

        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            rule->deactivate();
            if (rule->isLoaded()) {
                scalar activationDegree = rule->computeActivationDegree(conjunction, disjunction);
                rule->setActivationDegree(activationDegree);
                rulesToActivate.push(rule);
            }
        }

        int activated = 0;
        while (rulesToActivate.size() > 0 and activated++ < getActivatedRules()) {
            Rule* rule = rulesToActivate.top();
            rule->activate(rule->getActivationDegree(), implication);
            rulesToActivate.pop();
        }
    }

    Lowest* Lowest::clone() const {
        return new Lowest(*this);
    }

    Activation* Lowest::constructor() {
        return new Lowest;
    }

}
