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

#include "fl/activation/Highest.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/rule/Antecedent.h"
#include "fl/Operation.h"

#include <queue>

namespace fl {

    Highest::Highest(int numberOfRules) : Activation(), _activatedRules(numberOfRules) {
    }

    Highest::~Highest() {

    }

    std::string Highest::className() const {
        return "Highest";
    }

    std::string Highest::parameters() const {
        return Op::str(getActivatedRules());
    }

    void Highest::configure(const std::string& parameters) {
        setActivatedRules((int) Op::toScalar(parameters));
    }

    int Highest::getActivatedRules() const {
        return this->_activatedRules;
    }

    void Highest::setActivatedRules(int numberOfRules) {
        this->_activatedRules = numberOfRules;
    }

    struct RuleDegreeComparatorDescending {

        bool operator()(const Rule* a, const Rule* b) const {
            return a->getActivationDegree() < b->getActivationDegree();
        }
    };

    void Highest::activate(RuleBlock* ruleBlock) const {
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        std::priority_queue<Rule*, std::vector<Rule*>,
                RuleDegreeComparatorDescending> rulesToActivate;

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

    Highest* Highest::clone() const {
        return new Highest(*this);
    }

    Activation* Highest::constructor() {
        return new Highest;
    }





}
