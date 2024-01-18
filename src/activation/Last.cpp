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

#include "fl/activation/Last.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/Operation.h"

namespace fl {

    Last::Last(int numberOfRules, scalar threshold) : Activation(),
    _numberOfRules(numberOfRules), _threshold(threshold) { }

    Last::~Last() { }

    std::string Last::className() const {
        return "Last";
    }

    std::string Last::parameters() const {
        return Op::str(getNumberOfRules()) + " " + Op::str(getThreshold());
    }

    void Last::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] activation <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setNumberOfRules((int) Op::toScalar(values.at(0)));
        setThreshold(Op::toScalar(values.at(1)));
    }

    void Last::setNumberOfRules(int numberOfRules) {
        this->_numberOfRules = numberOfRules;
    }

    int Last::getNumberOfRules() const {
        return this->_numberOfRules;
    }

    void Last::setThreshold(scalar threshold) {
        this->_threshold = threshold;
    }

    scalar Last::getThreshold() const {
        return this->_threshold;
    }

    Complexity Last::complexity(const RuleBlock* ruleBlock) const {
        Complexity result;

        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        Complexity meanFiring;
        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            result.comparison(1 + 3);
            const Rule* rule = ruleBlock->getRule(i);
            result += rule->complexityOfActivation(conjunction, disjunction);
            meanFiring += rule->complexityOfFiring(implication);
        }
        meanFiring.divide(scalar(ruleBlock->numberOfRules()));

        result += meanFiring.multiply(getNumberOfRules());
        result += Complexity().arithmetic(1).multiply(getNumberOfRules());
        return result;
    }

    void Last::activate(RuleBlock* ruleBlock) {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        int activated = 0;
        for (std::vector<Rule*>::const_reverse_iterator it = ruleBlock->rules().rbegin();
                it != ruleBlock->rules().rend(); ++it) {
            Rule* rule = (*it);
            rule->deactivate();

            if (rule->isLoaded()) {
                scalar activationDegree = rule->activateWith(conjunction, disjunction);
                if (activated < _numberOfRules
                        and Op::isGt(activationDegree, 0.0)
                        and Op::isGE(activationDegree, _threshold)) {
                    rule->trigger(implication);
                    ++activated;
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
