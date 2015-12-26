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

#include "fl/activation/First.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/rule/Antecedent.h"
#include "fl/Operation.h"

#include <vector>

namespace fl {

    First::First(scalar threshold) : Activation(), _threshold(threshold) {

    }

    First::~First() {

    }

    std::string First::className() const {
        return "First";
    }

    std::string First::parameters() const {
        return Op::str(getThreshold());
    }

    void First::configure(const std::string& parameters) {
        setThreshold(Op::toScalar(parameters));
    }

    void First::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        Rule* first = fl::null;
        for (std::vector<Rule*>::const_iterator it = ruleBlock->rules().begin();
                it != ruleBlock->rules().end(); ++it) {
            Rule* rule = (*it);
            rule->deactivate();

            if (rule->isLoaded()) {
                scalar activationDegree = rule->computeActivationDegree(conjunction, disjunction);
                rule->setActivationDegree(activationDegree);
                if (not first and Op::isGt(activationDegree, 0.0)
                        and Op::isGE(activationDegree, getThreshold())) {
                    first = rule;
                    first->activate(activationDegree, implication);
                }
            }
        }
    }

    void First::setThreshold(scalar threshold) {
        this->_threshold = threshold;
    }

    scalar First::getThreshold() const {
        return this->_threshold;
    }

    First* First::clone() const {
        return new First(*this);
    }

    Activation* First::constructor() {
        return new First;
    }

}

