/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/activation/General.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/Operation.h"

namespace fl {

    General::General() : Activation() { }

    General::~General() { }

    std::string General::className() const {
        return "General";
    }

    std::string General::parameters() const {
        return "";
    }

    void General::configure(const std::string& parameters) {
        FL_IUNUSED(parameters);
    }

    Complexity General::complexity(const RuleBlock* ruleBlock) const {
        Complexity result;
        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            result.comparison(1);
            result += ruleBlock->getRule(i)->complexity(
                    ruleBlock->getConjunction(), ruleBlock->getDisjunction(),
                    ruleBlock->getImplication());
        }
        return result;
    }

    void General::activate(RuleBlock* ruleBlock) {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        const std::size_t numberOfRules = ruleBlock->numberOfRules();
        for (std::size_t i = 0; i < numberOfRules; ++i) {
            Rule* rule = ruleBlock->getRule(i);
            rule->deactivate();
            if (rule->isLoaded()) {
                rule->activateWith(conjunction, disjunction);
                rule->trigger(implication);
            }
        }
    }

    General* General::clone() const {
        return new General(*this);
    }

    Activation* General::constructor() {
        return new General;
    }

}
