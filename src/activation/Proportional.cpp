/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/activation/Proportional.h"

#include "fuzzylite/Operation.h"
#include "fuzzylite/rule/Rule.h"
#include "fuzzylite/rule/RuleBlock.h"

namespace fuzzylite {
    Proportional::Proportional() : Activation() {}

    Proportional::~Proportional() {}

    std::string Proportional::className() const {
        return "Proportional";
    }

    std::string Proportional::parameters() const {
        return "";
    }

    void Proportional::configure(const std::string& parameters) {
        FL_IUNUSED(parameters);
    }

    void Proportional::activate(RuleBlock* ruleBlock) {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        scalar sumActivationDegrees = 0.0;
        std::vector<Rule*> rulesToActivate;
        const std::size_t numberOfRules = ruleBlock->numberOfRules();
        for (std::size_t i = 0; i < numberOfRules; ++i) {
            Rule* rule = ruleBlock->getRule(i);
            rule->deactivate();
            if (rule->isLoaded()) {
                scalar activationDegree = rule->activateWith(conjunction, disjunction);
                rulesToActivate.push_back(rule);
                sumActivationDegrees += activationDegree;
            }
        }

        for (std::size_t i = 0; i < rulesToActivate.size(); ++i) {
            Rule* rule = rulesToActivate.at(i);
            scalar activationDegree = rule->getActivationDegree() / sumActivationDegrees;
            rule->setActivationDegree(Op::isNaN(activationDegree) ? 0.0 : activationDegree);
            rule->trigger(implication);
        }
    }

    Proportional* Proportional::clone() const {
        return new Proportional(*this);
    }

    Activation* Proportional::constructor() {
        return new Proportional;
    }
}
