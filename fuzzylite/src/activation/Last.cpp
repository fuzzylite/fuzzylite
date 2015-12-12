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

#include "fl/activation/Last.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/rule/Antecedent.h"
#include "fl/Operation.h"

#include <vector>

namespace fl {

    Last::Last() : Activation() {

    }

    Last::~Last() {

    }

    std::string Last::className() const {
        return "Last";
    }

    std::string Last::parameters() const {
        return "";
    }

    void Last::configure(const std::string& parameters) {
        (void) parameters;
    }

    void Last::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        bool activated = false;
        for (std::vector<Rule*>::const_reverse_iterator it = ruleBlock->rules().rbegin();
                it != ruleBlock->rules().rend(); ++it) {
            Rule* rule = (*it);
            if (rule->isLoaded() and not activated) {
                scalar activationDegree = rule->getWeight()
                        * rule->getAntecedent()->activationDegree(conjunction, disjunction);
                rule->activate(activationDegree, implication);
                activated = Op::isGt(activationDegree, 0.0);
            } else {
                rule->deactivate();
            }
        }
    }

    Last* Last::clone() const {
        return new Last(*this);
    }

    Activation* Last::constructor() {
        return new Last;
    }

}


