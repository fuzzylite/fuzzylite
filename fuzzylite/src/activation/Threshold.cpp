/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
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
        ss << getComparisonOperator() << " " << Op::str(_threshold);
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
        setComparison(parseComparisonOperator(values.at(0)));
        setThreshold(Op::toScalar(values.at(1)));
    }

    void Threshold::setComparison(Comparison comparison) {
        this->_comparison = comparison;
    }

    Threshold::Comparison Threshold::getComparison() const {
        return this->_comparison;
    }

    std::string Threshold::getComparisonOperator() const {
        switch (_comparison) {
            case EqualTo: return "==";
            case NotEqualTo: return "!=";
            case LessThan: return "<";
            case LessThanOrEqualTo: return "<=";
            case GreaterThan: return ">";
            case GreaterThanOrEqualTo: return ">=";
            default: return "[invalid threshold type]";
        }
    }

    Threshold::Comparison Threshold::parseComparisonOperator(const std::string& name) const {
        if (name == "==") return EqualTo;
        if (name == "!=") return NotEqualTo;
        if (name == "<") return LessThan;
        if (name == "<=") return LessThanOrEqualTo;
        if (name == ">") return GreaterThan;
        if (name == ">=") return GreaterThanOrEqualTo;
        throw fl::Exception("[syntax error] invalid threshold type by name <" + name + ">", FL_AT);
    }

    void Threshold::setThreshold(scalar threshold) {
        this->_threshold = threshold;
    }

    scalar Threshold::getThreshold() const {
        return this->_threshold;
    }

    bool Threshold::activates(scalar activationDegree) const {
        switch (_comparison) {
            case EqualTo: return Op::isEq(activationDegree, _threshold);
            case NotEqualTo: return not Op::isEq(activationDegree, _threshold);
            case LessThan: return Op::isLt(activationDegree, _threshold);
            case LessThanOrEqualTo: return Op::isLE(activationDegree, _threshold);
            case GreaterThan: return Op::isGt(activationDegree, _threshold);
            case GreaterThanOrEqualTo: return Op::isGE(activationDegree, _threshold);
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
            if (rule->isLoaded()) {
                scalar activationDegree = rule->getWeight()
                        * rule->getAntecedent()->activationDegree(conjunction, disjunction);
                if (activates(activationDegree)) {
                    rule->activate(activationDegree, implication);
                } else {
                    rule->deactivate();
                }
            } else {
                rule->deactivate();
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

