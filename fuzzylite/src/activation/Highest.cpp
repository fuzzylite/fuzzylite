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

#include "fl/activation/Highest.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/Operation.h"

#include <queue>

namespace fl {

    Highest::Highest(int numberOfRules) : Activation(), _numberOfRules(numberOfRules) { }

    Highest::~Highest() { }

    std::string Highest::className() const {
        return "Highest";
    }

    std::string Highest::parameters() const {
        return Op::str(getNumberOfRules());
    }

    void Highest::configure(const std::string& parameters) {
        setNumberOfRules((int) Op::toScalar(parameters));
    }

    int Highest::getNumberOfRules() const {
        return this->_numberOfRules;
    }

    void Highest::setNumberOfRules(int numberOfRules) {
        this->_numberOfRules = numberOfRules;
    }

    Complexity Highest::complexity(const RuleBlock* ruleBlock) const {
        //Cost of priority_queue:
        //http://stackoverflow.com/questions/2974470/efficiency-of-the-stl-priority-queue
        Complexity result;

        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        Complexity meanFiring;
        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            const Rule* rule = ruleBlock->getRule(i);
            result += rule->complexityOfActivation(conjunction, disjunction);
            meanFiring += rule->complexityOfFiring(implication);
        }
        meanFiring.divide(scalar(ruleBlock->numberOfRules()));

        //Complexity of push is O(log n)
        result += Complexity().function(1).multiply(ruleBlock->numberOfRules()
                * std::log(scalar(ruleBlock->numberOfRules())));

        result += Complexity().comparison(2).arithmetic(1).multiply(getNumberOfRules());
        result += meanFiring.multiply(getNumberOfRules());
        //Complexity of pop is 2 * O(log n)
        result += Complexity().function(1).multiply(getNumberOfRules() *
                2 * std::log(scalar(ruleBlock->numberOfRules())));
        return result;
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
