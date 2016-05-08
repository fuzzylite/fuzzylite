/*
 Copyright © 2010-2016 by FuzzyLite Limited.
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

    First::First(int numberOfRules, scalar threshold) : Activation(),
    _numberOfRules(numberOfRules), _threshold(threshold) {

    }

    First::~First() {

    }

    std::string First::className() const {
        return "First";
    }

    std::string First::parameters() const {
        return Op::str(getNumberOfRules()) + " " + Op::str(getThreshold());
    }

    void First::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] activation <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setNumberOfRules((int) Op::toScalar(values.at(0)));
        setThreshold(Op::toScalar(values.at(1)));
    }

    void First::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        int activated = 0;
        for (std::vector<Rule*>::const_iterator it = ruleBlock->rules().begin();
                it != ruleBlock->rules().end(); ++it) {
            Rule* rule = (*it);
            rule->deactivate();

            if (rule->isLoaded()) {
                scalar activationDegree = rule->computeActivationDegree(conjunction, disjunction);
                rule->setActivationDegree(activationDegree);
                if (activated < getNumberOfRules()
                        and Op::isGt(activationDegree, 0.0)
                        and Op::isGE(activationDegree, getThreshold())) {
                    rule->activate(activationDegree, implication);
                    ++activated;
                }
            }
        }
    }

    void First::setNumberOfRules(int numberOfRules) {
        this->_numberOfRules = numberOfRules;
    }

    int First::getNumberOfRules() const {
        return this->_numberOfRules;
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

