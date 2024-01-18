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

#include "fl/imex/FllExporter.h"

#include "fl/Headers.h"

namespace fl {

    FllExporter::FllExporter(const std::string& indent, const std::string& separator)
    : Exporter(), _indent(indent), _separator(separator) { }

    FllExporter::~FllExporter() { }

    std::string FllExporter::name() const {
        return "FllExporter";
    }

    void FllExporter::setIndent(const std::string& indent) {
        this->_indent = indent;
    }

    std::string FllExporter::getIndent() const {
        return this->_indent;
    }

    void FllExporter::setSeparator(const std::string& separator) {
        this->_separator = separator;
    }

    std::string FllExporter::getSeparator() const {
        return this->_separator;
    }

    std::string FllExporter::toString(const Engine* engine) const {
        std::vector<std::string> result;
        result.push_back("Engine: " + engine->getName());
        if (not engine->getDescription().empty())
            result.push_back("description: " + engine->getDescription());
        for (std::size_t i = 0 ; i < engine->numberOfInputVariables(); ++i){
            result.push_back(toString(engine->getInputVariable(i)));
        }
        for (std::size_t i = 0 ; i < engine->numberOfOutputVariables(); ++i){
            result.push_back(toString(engine->getOutputVariable(i)));
        }
        for (std::size_t i = 0 ; i < engine->numberOfRuleBlocks(); ++i){
            result.push_back(toString(engine->getRuleBlock(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const std::vector<Variable*>& variables) const {
        std::vector<std::string> result;
        for (std::size_t i = 0; i < variables.size(); ++i) {
            result.push_back(toString(variables.at(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const std::vector<InputVariable*>& variables) const {
        std::vector<std::string> result;
        for (std::size_t i = 0; i < variables.size(); ++i) {
            result.push_back(toString(variables.at(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const std::vector<OutputVariable*>& variables) const {
        std::vector<std::string> result;
        for (std::size_t i = 0; i < variables.size(); ++i) {
            result.push_back(toString(variables.at(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const std::vector<RuleBlock*>& ruleBlocks) const {
        std::vector<std::string> result;
        for (std::size_t i = 0; i < ruleBlocks.size(); ++i) {
            result.push_back(toString(ruleBlocks.at(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const Variable* variable) const {
        std::vector<std::string> result;
        result.push_back("Variable: " + Op::validName(variable->getName()));
        if (not variable->getDescription().empty()) {
            result.push_back(_indent + "description: " + variable->getDescription());
        }
        result.push_back(_indent + "enabled: " + (variable->isEnabled() ? "true" : "false"));
        result.push_back(_indent + "range: " + Op::join(2, " ",
                variable->getMinimum(), variable->getMaximum()));
        result.push_back(_indent + "lock-range: " +
                (variable->isLockValueInRange() ? "true" : "false"));
        for (std::size_t i = 0; i < variable->numberOfTerms(); ++i) {
            result.push_back(_indent + toString(variable->getTerm(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const InputVariable* inputVariable) const {
        std::vector<std::string> result;
        result.push_back("InputVariable: " + Op::validName(inputVariable->getName()));
        if (not inputVariable->getDescription().empty()) {
            result.push_back(_indent + "description: " + inputVariable->getDescription());
        }
        result.push_back(_indent + "enabled: " + (inputVariable->isEnabled() ? "true" : "false"));
        result.push_back(_indent + "range: " + Op::join(2, " ",
                inputVariable->getMinimum(), inputVariable->getMaximum()));
        result.push_back(_indent + "lock-range: " +
                (inputVariable->isLockValueInRange() ? "true" : "false"));
        for (std::size_t i = 0; i < inputVariable->numberOfTerms(); ++i) {
            result.push_back(_indent + toString(inputVariable->getTerm(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const OutputVariable* outputVariable) const {
        std::vector<std::string> result;
        result.push_back("OutputVariable: " + Op::validName(outputVariable->getName()));
        if (not outputVariable->getDescription().empty()) {
            result.push_back(_indent + "description: " + outputVariable->getDescription());
        }
        result.push_back(_indent + "enabled: " + (outputVariable->isEnabled() ? "true" : "false"));
        result.push_back(_indent + "range: " + Op::join(2, " ",
                outputVariable->getMinimum(), outputVariable->getMaximum()));
        result.push_back(_indent + "lock-range: " +
                (outputVariable->isLockValueInRange() ? "true" : "false"));
        result.push_back(_indent + "aggregation: " +
                toString(outputVariable->fuzzyOutput()->getAggregation()));
        result.push_back(_indent + "defuzzifier: " +
                toString(outputVariable->getDefuzzifier()));
        result.push_back(_indent + "default: " + Op::str(outputVariable->getDefaultValue()));
        result.push_back(_indent + "lock-previous: " +
                (outputVariable->isLockPreviousValue() ? "true" : "false"));
        for (std::size_t i = 0; i < outputVariable->numberOfTerms(); ++i) {
            result.push_back(_indent + toString(outputVariable->getTerm(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const RuleBlock* ruleBlock) const {
        std::vector<std::string> result;
        result.push_back("RuleBlock: " + ruleBlock->getName());
        if (not ruleBlock->getDescription().empty()) {
            result.push_back(_indent + "description: " + ruleBlock->getDescription());
        }
        result.push_back(_indent + "enabled: " +
                (ruleBlock->isEnabled() ? "true" : "false"));
        result.push_back(_indent + "conjunction: " + toString(ruleBlock->getConjunction()));
        result.push_back(_indent + "disjunction: " + toString(ruleBlock->getDisjunction()));
        result.push_back(_indent + "implication: " + toString(ruleBlock->getImplication()));
        result.push_back(_indent + "activation: " + toString(ruleBlock->getActivation()));
        for (std::size_t i = 0; i < ruleBlock->numberOfRules(); ++i) {
            result.push_back(_indent + toString(ruleBlock->getRule(i)));
        }
        return Op::join(result, _separator);
    }

    std::string FllExporter::toString(const Rule* rule) const {
        return "rule: " + rule->getText();
    }

    std::string FllExporter::toString(const Term* term) const {
        return "term: " + Op::validName(term->getName()) + " " + term->className()
                + " " + term->parameters();
    }

    std::string FllExporter::toString(const Norm* norm) const {
        if (norm) return norm->className();
        return "none";
    }

    std::string FllExporter::toString(const Activation* activation) const {
        if (not activation) return "none";
        if (activation->parameters().empty()) return activation->className();
        return activation->className() + " " + activation->parameters();
    }

    std::string FllExporter::toString(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) return "none";
        if (const IntegralDefuzzifier * integralDefuzzifier =
                dynamic_cast<const IntegralDefuzzifier*> (defuzzifier)) {
            return defuzzifier->className() + " " + Op::str(integralDefuzzifier->getResolution());

        } else if (const WeightedDefuzzifier * weightedDefuzzifier =
                dynamic_cast<const WeightedDefuzzifier*> (defuzzifier)) {
            return weightedDefuzzifier->className() + " " + weightedDefuzzifier->getTypeName();
        }
        return defuzzifier->className();
    }

    FllExporter* FllExporter::clone() const {
        return new FllExporter(*this);
    }

}
