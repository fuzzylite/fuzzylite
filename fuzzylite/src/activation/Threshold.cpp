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

#include "fl/activation/Threshold.h"

#include "fl/rule/RuleBlock.h"
#include "fl/rule/Rule.h"
#include "fl/rule/Antecedent.h"
#include "fl/Operation.h"

namespace fl {

    Threshold::Threshold(Comparison comparison, scalar threshold) : Activation(),
    _comparison(comparison), _threshold(threshold) {

    }

    Threshold::~Threshold() {

    }

    std::string Threshold::className() const {
        return "Threshold";
    }

    std::string Threshold::parameters() const {
        std::ostringstream ss;
        ss << comparisonShortForm() << " " << Op::str(getThreshold());
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
            throw fl::Exception(ex.str(), FL_AT);
        }
        setComparison(parseComparison(values.at(0)));
        setThreshold(Op::toScalar(values.at(1)));
    }

    void Threshold::setComparison(Comparison comparison) {
        this->_comparison = comparison;
    }

    Threshold::Comparison Threshold::getComparison() const {
        return this->_comparison;
    }

    std::string Threshold::comparisonShortForm() const {
        return comparisonShortForm(getComparison());
    }

    std::string Threshold::comparisonShortForm(Comparison comparison) const {
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

    std::vector<std::string> Threshold::availableComparisons() const {
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
        throw fl::Exception("[syntax error] invalid threshold type by name <" + name + ">", FL_AT);
    }

    void Threshold::setThreshold(scalar threshold) {
        this->_threshold = threshold;
    }

    scalar Threshold::getThreshold() const {
        return this->_threshold;
    }

    void Threshold::setComparisonThreshold(Comparison comparison, scalar threshold) {
        setComparison(comparison);
        setThreshold(threshold);
    }

    void Threshold::setComparisonThreshold(const std::string& comparison, scalar threshold) {
        setComparison(parseComparison(comparison));
        setThreshold(threshold);
    }

    bool Threshold::activatesWith(scalar activationDegree) const {
        switch (getComparison()) {
            case LessThan: return Op::isLt(activationDegree, getThreshold());
            case LessThanOrEqualTo: return Op::isLE(activationDegree, getThreshold());
            case EqualTo: return Op::isEq(activationDegree, getThreshold());
            case NotEqualTo: return not Op::isEq(activationDegree, getThreshold());
            case GreaterThanOrEqualTo: return Op::isGE(activationDegree, getThreshold());
            case GreaterThan: return Op::isGt(activationDegree, getThreshold());
            default: return false;
        }
    }

    void Threshold::activate(RuleBlock* ruleBlock) const {
        FL_DBG("Activation: " << className() << " " << parameters());
        const TNorm* conjunction = ruleBlock->getConjunction();
        const SNorm* disjunction = ruleBlock->getDisjunction();
        const TNorm* implication = ruleBlock->getImplication();

        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            rule->deactivate();
            if (rule->isLoaded()) {
                scalar activationDegree = rule->computeActivationDegree(conjunction, disjunction);
                rule->setActivationDegree(activationDegree);
                if (activatesWith(activationDegree)) {
                    rule->activate(activationDegree, implication);
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

