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

#include "fl/activation/Probabilistic.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/rule/Antecedent.h"
#include "fl/Operation.h"

#include <vector>

namespace fl {

    Probabilistic::Probabilistic() : Activation() {
    }

    Probabilistic::~Probabilistic() {

    }

    std::string Probabilistic::className() const {
        return "Probabilistic";
    }

    std::string Probabilistic::parameters() const {
        return "";
    }

    void Probabilistic::configure(const std::string& parameters) {
        FL_IUNUSED(parameters);
    }

    void Probabilistic::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            rule->deactivate();
            scalar probability = (Op::random() % 100) / 100.0;
            if (rule->isLoaded() and Op::isLE(probability, rule->getWeight())) {
                scalar activationDegree = rule->getAntecedent()->activationDegree(conjunction, disjunction);
                rule->activate(activationDegree, implication);
            }
        }
    }

    Probabilistic* Probabilistic::clone() const {
        return new Probabilistic(*this);
    }

    Activation* Probabilistic::constructor() {
        return new Probabilistic;
    }





}
