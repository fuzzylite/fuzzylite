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

#include "fuzzylite/activation/Lowest.h"

#include <queue>

#include "fuzzylite/Operation.h"
#include "fuzzylite/rule/Rule.h"
#include "fuzzylite/rule/RuleBlock.h"

namespace fuzzylite {
    Lowest::Lowest(int numberOfRules) : Activation(), _numberOfRules(numberOfRules) {}

    Lowest::~Lowest() {}

    std::string Lowest::className() const {
        return "Lowest";
    }

    std::string Lowest::parameters() const {
        return Op::str(getNumberOfRules());
    }

    void Lowest::configure(const std::string& parameters) {
        if (parameters.empty())
            return;
        const std::vector<std::string> values = Op::split(parameters, " ");
        if (not values.empty())
            setNumberOfRules((int)Op::toScalar(values.front()));
    }

    int Lowest::getNumberOfRules() const {
        return this->_numberOfRules;
    }

    void Lowest::setNumberOfRules(int activatedRules) {
        this->_numberOfRules = activatedRules;
    }

    struct Ascending {
        bool operator()(const Rule* a, const Rule* b) {
            return a->getActivationDegree() > b->getActivationDegree();
        }
    };

    void Lowest::activate(RuleBlock* ruleBlock) {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        std::priority_queue<Rule*, std::vector<Rule*>, Ascending> rulesToActivate;

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

    Lowest* Lowest::clone() const {
        return new Lowest(*this);
    }

    Activation* Lowest::constructor() {
        return new Lowest;
    }
}
