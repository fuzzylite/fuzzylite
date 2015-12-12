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
        return Op::str(_activatedRules);
    }

    void Highest::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        setActivatedRules((int) Op::toScalar(parameters));
    }

    int Highest::getActivatedRules() const {
        return this->_activatedRules;
    }

    void Highest::setActivatedRules(int numberOfRules) {
        this->_activatedRules = numberOfRules;
    }

    typedef std::pair<Rule*, scalar> RuleDegree;

    struct RuleDegreeComparatorDescending {

        bool operator()(const RuleDegree& a, const RuleDegree& b) const {
            return a.second < b.second;
        }
    };

    void Highest::activate(RuleBlock* ruleBlock) const {
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        std::priority_queue<RuleDegree, std::vector<RuleDegree>,
                RuleDegreeComparatorDescending> rulesToActivate;

        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            if (rule->isLoaded()) {
                scalar activationDegree = rule->getWeight()
                        * rule->getAntecedent()->activationDegree(conjunction, disjunction);
                if (_activatedRules < 0) {
                    //sort ascending to activate the lowest n rules
                    activationDegree = -activationDegree;
                }
                rulesToActivate.push(RuleDegree(rule, activationDegree));
            } else {
                rule->deactivate();
            }
        }

        int activated = 0;
        while (rulesToActivate.size() > 0 and activated++ < _activatedRules) {
            Rule* rule = rulesToActivate.top().first;
            scalar activationDegree = std::fabs(rulesToActivate.top().second);
            rule->activate(activationDegree, implication);
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
