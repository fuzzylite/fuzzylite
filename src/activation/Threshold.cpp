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

#include "fl/activation/Threshold.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/Operation.h"

namespace fl {

    Threshold::Threshold(Comparison comparison, scalar threshold) : Activation(),
    _comparison(comparison), _value(threshold) { }

    Threshold::Threshold(const std::string& comparison, scalar threshold) : Activation(),
    _comparison(parseComparison(comparison)), _value(threshold) { }

    Threshold::~Threshold() { }

    std::string Threshold::className() const {
        return "Threshold";
    }

    std::string Threshold::parameters() const {
        std::ostringstream ss;
        ss << comparisonOperator() << " " << Op::str(getValue());
        return ss.str();
    }

    void Threshold::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] activation <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setComparison(parseComparison(values.at(0)));
        setValue(Op::toScalar(values.at(1)));
    }

    void Threshold::setComparison(Comparison comparison) {
        this->_comparison = comparison;
    }

    Threshold::Comparison Threshold::getComparison() const {
        return this->_comparison;
    }

    std::string Threshold::comparisonOperator() const {
        return comparisonOperator(getComparison());
    }

    std::string Threshold::comparisonOperator(Comparison comparison) const {
        switch (comparison) {
            case LessThan: return "<";
            case LessThanOrEqualTo: return "<=";
            case EqualTo: return "==";
            case NotEqualTo: return "!=";
            case GreaterThanOrEqualTo: return ">=";
            case GreaterThan: return ">";
            default: return "?";
        }
    }

    std::vector<std::string> Threshold::availableComparisonOperators() const {
        std::vector<std::string> result;
        result.push_back("<");
        result.push_back("<=");
        result.push_back("==");
        result.push_back("!=");
        result.push_back(">=");
        result.push_back(">");
        return result;
    }

    Threshold::Comparison Threshold::parseComparison(const std::string& name) const {
        if (name == "<") return LessThan;
        if (name == "<=") return LessThanOrEqualTo;
        if (name == "==") return EqualTo;
        if (name == "!=") return NotEqualTo;
        if (name == ">=") return GreaterThanOrEqualTo;
        if (name == ">") return GreaterThan;
        throw Exception("[syntax error] invalid threshold type by name <" + name + ">", FL_AT);
    }

    void Threshold::setValue(scalar value) {
        this->_value = value;
    }

    scalar Threshold::getValue() const {
        return this->_value;
    }

    void Threshold::setThreshold(Comparison comparison, scalar threshold) {
        setComparison(comparison);
        setValue(threshold);
    }

    void Threshold::setThreshold(const std::string& comparison, scalar value) {
        setComparison(parseComparison(comparison));
        setValue(value);
    }

    bool Threshold::activatesWith(scalar activationDegree) const {
        switch (getComparison()) {
            case LessThan: return Op::isLt(activationDegree, getValue());
            case LessThanOrEqualTo: return Op::isLE(activationDegree, getValue());
            case EqualTo: return Op::isEq(activationDegree, getValue());
            case NotEqualTo: return not Op::isEq(activationDegree, getValue());
            case GreaterThanOrEqualTo: return Op::isGE(activationDegree, getValue());
            case GreaterThan: return Op::isGt(activationDegree, getValue());
            default: return false;
        }
    }

    Complexity Threshold::complexity(const RuleBlock* ruleBlock) const {
        Complexity result;
        for (std::size_t i = 0; i < ruleBlock->rules().size(); ++i) {
            result.comparison(2);
            result += ruleBlock->rules().at(i)->complexity(
                    ruleBlock->getConjunction(), ruleBlock->getDisjunction(),
                    ruleBlock->getImplication());
        }
        return result;
    }

    void Threshold::activate(RuleBlock* ruleBlock) {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            rule->deactivate();
            if (rule->isLoaded()) {
                scalar activationDegree = rule->activateWith(conjunction, disjunction);
                if (activatesWith(activationDegree)) {
                    rule->trigger(implication);
                }
            }
        }
    }

    Threshold* Threshold::clone() const {
        return new Threshold(*this);
    }

    Activation* Threshold::constructor() {
        return new Threshold;
    }

}

