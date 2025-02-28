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

#include "fuzzylite/activation/Highest.h"

#include <queue>

#include "fuzzylite/Operation.h"
#include "fuzzylite/rule/Rule.h"
#include "fuzzylite/rule/RuleBlock.h"

namespace fuzzylite {
    Highest::Highest(int numberOfRules) : Activation(), _numberOfRules(numberOfRules) {}

    Highest::~Highest() {}

    std::string Highest::className() const {
        return "Highest";
    }

    std::string Highest::parameters() const {
        return Op::str(getNumberOfRules());
    }

    void Highest::configure(const std::string& parameters) {
        if (parameters.empty())
            return;
        const std::vector<std::string> values = Op::split(parameters, " ");
        if (not values.empty())
            setNumberOfRules((int)Op::toScalar(values.front()));
    }

    int Highest::getNumberOfRules() const {
        return this->_numberOfRules;
    }

    void Highest::setNumberOfRules(int numberOfRules) {
        this->_numberOfRules = numberOfRules;
    }

    struct Descending {
        bool operator()(const Rule* a, const Rule* b) const {
            return a->getActivationDegree() < b->getActivationDegree();
        }
    };

    void Highest::activate(RuleBlock* ruleBlock) {
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        std::priority_queue<Rule*, std::vector<Rule*>, Descending> rulesToActivate;

        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            rule->deactivate();
            if (rule->isLoaded()) {
                scalar activationDegree = rule->activateWith(conjunction, disjunction);
                if (Op::isGt(activationDegree, 0.0))
                    rulesToActivate.push(rule);
            }
        }

        int activated = 0;
        while (rulesToActivate.size() > 0 and activated++ < _numberOfRules) {
            Rule* rule = rulesToActivate.top();
            rule->trigger(implication);
            rulesToActivate.pop();
        }
    }

    Highest* Highest::clone() const {
        return new Highest(*this);
    }

    Activation* Highest::constructor() {
        return new Highest;
    }
}
