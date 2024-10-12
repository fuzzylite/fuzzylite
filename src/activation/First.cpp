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

#include "fuzzylite/activation/First.h"

#include "fuzzylite/Operation.h"
#include "fuzzylite/rule/Rule.h"
#include "fuzzylite/rule/RuleBlock.h"

namespace fuzzylite {

    First::First(int numberOfRules, scalar threshold) :
        Activation(),
        _numberOfRules(numberOfRules),
        _threshold(threshold) {}

    First::~First() {}

    std::string First::className() const {
        return "First";
    }

    std::string First::parameters() const {
        return Op::str(getNumberOfRules()) + " " + Op::str(getThreshold());
    }

    void First::configure(const std::string& parameters) {
        if (parameters.empty())
            return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] activation <" << className() << ">" << " requires <" << required
               << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setNumberOfRules((int)Op::toScalar(values.at(0)));
        setThreshold(Op::toScalar(values.at(1)));
    }

    void First::activate(RuleBlock* ruleBlock) {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        int activated = 0;
        for (std::vector<Rule*>::const_iterator it = ruleBlock->rules().begin(); it != ruleBlock->rules().end(); ++it) {
            Rule* rule = (*it);
            rule->deactivate();

            if (rule->isLoaded()) {
                scalar activationDegree = rule->activateWith(conjunction, disjunction);
                if (activated < _numberOfRules and Op::isGt(activationDegree, 0.0)
                    and Op::isGE(activationDegree, _threshold)) {
                    rule->trigger(implication);
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
