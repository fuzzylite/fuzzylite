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

#include "fl/activation/Proportional.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/rule/Antecedent.h"
#include "fl/Operation.h"

#include <vector>

namespace fl {

    Proportional::Proportional() : Activation() {
    }

    Proportional::~Proportional() {

    }

    std::string Proportional::className() const {
        return "Proportional";
    }

    std::string Proportional::parameters() const {
        return "";
    }

    void Proportional::configure(const std::string& parameters) {
        (void) parameters;
    }

    void Proportional::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        typedef std::pair<Rule*, scalar> RuleDegree;

        scalar sumActivationDegrees = 0.0;
        std::vector<RuleDegree> rulesToActivate(ruleBlock->numberOfRules());
        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            if (rule->isLoaded()) {
                scalar activationDegree = rule->computeActivationDegree(conjunction, disjunction);
                rulesToActivate.push_back(RuleDegree(rule, activationDegree));
                sumActivationDegrees += activationDegree;
            } else {
                rule->deactivate();
            }
        }

        for (std::size_t i = 0; i < rulesToActivate.size(); ++i) {
            Rule* rule = rulesToActivate.at(i).first;
            scalar activationDegree = rulesToActivate.at(i).second / sumActivationDegrees;
            rule->activate(activationDegree, implication);
        }
    }

    Proportional* Proportional::clone() const {
        return new Proportional(*this);
    }

    Activation* Proportional::constructor() {
        return new Proportional;
    }





}
