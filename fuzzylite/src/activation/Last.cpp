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

    Last::Last(scalar threshold) : Activation(), _threshold(threshold) {

    }

    Last::~Last() {

    }

    std::string Last::className() const {
        return "Last";
    }

    std::string Last::parameters() const {
        return Op::str(getThreshold());
    }

    void Last::configure(const std::string& parameters) {
        setThreshold(Op::toScalar(parameters));
    }

    void Last::setThreshold(scalar threshold) {
        this->_threshold = threshold;
    }

    scalar Last::getThreshold() const {
        return this->_threshold;
    }

    void Last::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        Rule* last = fl::null;
        for (std::vector<Rule*>::const_reverse_iterator it = ruleBlock->rules().rbegin();
                it != ruleBlock->rules().rend(); ++it) {
            Rule* rule = (*it);
            rule->deactivate();

            if (rule->isLoaded()) {
                scalar activationDegree = rule->computeActivationDegree(conjunction, disjunction);
                rule->setActivationDegree(activationDegree);
                if (not last and Op::isGt(activationDegree, 0.0)
                        and Op::isGE(activationDegree, getThreshold())) {
                    last = rule;
                    last->activate(activationDegree, implication);
                }
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


