/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2016 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
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
        FL_IUNUSED(parameters);
    }

    Complexity Proportional::complexity(const RuleBlock* ruleBlock) const {
        Complexity result;
        for (std::size_t i = 0; i < ruleBlock->rules().size(); ++i) {
            result.comparison(1).arithmetic(1);
            result += ruleBlock->rules().at(i)->complexityOfActivationDegree(
                    ruleBlock->getConjunction(), ruleBlock->getDisjunction(),
                    ruleBlock->getImplication());
        }

        for (std::size_t i = 0; i < ruleBlock->rules().size(); ++i) {
            result.arithmetic(1);
            result += ruleBlock->rules().at(i)
                    ->complexityOfActivation(ruleBlock->getImplication());
        }
        return result;
    }

    void Proportional::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        scalar sumActivationDegrees = 0.0;
        std::vector<Rule*> rulesToActivate;
        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            rule->deactivate();
            if (rule->isLoaded()) {
                scalar activationDegree = rule->computeActivationDegree(conjunction, disjunction);
                rule->setActivationDegree(activationDegree);
                rulesToActivate.push_back(rule);
                sumActivationDegrees += activationDegree;
            }
        }

        for (std::size_t i = 0; i < rulesToActivate.size(); ++i) {
            Rule* rule = rulesToActivate.at(i);
            scalar activationDegree = rule->getActivationDegree() / sumActivationDegrees;
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
