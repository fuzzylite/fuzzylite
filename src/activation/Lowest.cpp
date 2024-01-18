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

#include "fl/activation/Lowest.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/Operation.h"

#include <queue>

namespace fl {

    Lowest::Lowest(int numberOfRules) : Activation(), _numberOfRules(numberOfRules) { }

    Lowest::~Lowest() { }

    std::string Lowest::className() const {
        return "Lowest";
    }

    std::string Lowest::parameters() const {
        return Op::str(getNumberOfRules());
    }

    void Lowest::configure(const std::string& parameters) {
        setNumberOfRules((int) Op::toScalar(parameters));
    }

    int Lowest::getNumberOfRules() const {
        return this->_numberOfRules;
    }

    void Lowest::setNumberOfRules(int activatedRules) {
        this->_numberOfRules = activatedRules;
    }

    Complexity Lowest::complexity(const RuleBlock* ruleBlock) const {
        //Cost of priority_queue:
        //http://stackoverflow.com/questions/2974470/efficiency-of-the-stl-priority-queue
        Complexity result;

        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        Complexity meanFiring;
        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            const Rule* rule = ruleBlock->getRule(i);
            result.comparison(2);
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
