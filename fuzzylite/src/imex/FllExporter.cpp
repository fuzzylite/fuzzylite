/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/* 
 * File:   FllExporter.cpp
 * Author: jcrada
 *
 * Created on 18 December 2013, 8:58 AM
 */

#include "fl/imex/FllExporter.h"

#include "fl/Headers.h"

namespace fl {

    FllExporter::FllExporter(const std::string& indent, const std::string& separator)
    : Exporter(), _indent(indent), _separator(separator) {
    }

    FllExporter::~FllExporter() {
    }

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
        std::ostringstream ss;
        ss << "Engine: " << engine->getName() << _separator;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            ss << toString(engine->getInputVariable(i));
        }
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            ss << toString(engine->getOutputVariable(i));
        }
        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            ss << toString(engine->getRuleBlock(i));
        }
        return ss.str();
    }

    std::string FllExporter::toString(const Variable* variable) const {
        std::ostringstream ss;
        ss << "Variable: " << variable->getName() << _separator;
        std::string tab = _indent;
        ss << tab << "enabled: " << (variable->isEnabled() ? "true" : "false") << _separator;
        ss << tab << "range: " << Op::str(variable->getMinimum())
                << " " << Op::str(variable->getMaximum()) << _separator;
        for (int i = 0; i < variable->numberOfTerms(); ++i) {
            ss << tab << toString(variable->getTerm(i)) << _separator;
        }
        return ss.str();
    }

    std::string FllExporter::toString(const InputVariable* inputVariable) const {
        std::ostringstream ss;
        ss << "InputVariable: " << inputVariable->getName() << _separator;
        std::string tab = _indent;
        ss << tab << "enabled: " << (inputVariable->isEnabled() ? "true" : "false") << _separator;
        ss << tab << "range: " << Op::str(inputVariable->getMinimum())
                << " " << Op::str(inputVariable->getMaximum()) << _separator;
        for (int i = 0; i < inputVariable->numberOfTerms(); ++i) {
            ss << tab << toString(inputVariable->getTerm(i)) << _separator;
        }
        return ss.str();
    }

    std::string FllExporter::toString(const OutputVariable* outputVariable) const {
        std::ostringstream ss;
        ss << "OutputVariable: " << outputVariable->getName() << _separator;
        std::string tab = _indent;
        ss << tab << "enabled: " << (outputVariable->isEnabled() ? "true" : "false") << _separator;
        ss << tab << "range: " << Op::str(outputVariable->getMinimum())
                << " " << Op::str(outputVariable->getMaximum()) << _separator;
        ss << tab << "accumulation: " << toString(outputVariable->fuzzyOutput()->getAccumulation()) << _separator;
        ss << tab << "defuzzifier: " << toString(outputVariable->getDefuzzifier()) << _separator;
        ss << tab << "default: " << Op::str(outputVariable->getDefaultValue()) << _separator;
        ss << tab << "lock-valid: " << (outputVariable->isLockingValidOutput() ? "true" : "false") << _separator;
        ss << tab << "lock-range: " << (outputVariable->isLockingOutputRange() ? "true" : "false") << _separator;
        for (int i = 0; i < outputVariable->numberOfTerms(); ++i) {
            ss << tab << toString(outputVariable->getTerm(i)) << _separator;
        }
        return ss.str();
    }

    std::string FllExporter::toString(const RuleBlock* ruleBlock) const {
        std::ostringstream ss;
        ss << "RuleBlock: " << ruleBlock->getName() << _separator;
        std::string tab = _indent;
        ss << tab << "enabled: " << (ruleBlock->isEnabled() ? "true" : "false") << _separator;
        ss << tab << "conjunction: " << toString(ruleBlock->getConjunction()) << _separator;
        ss << tab << "disjunction: " << toString(ruleBlock->getDisjunction()) << _separator;
        ss << tab << "activation: " << toString(ruleBlock->getActivation()) << _separator;
        for (int i = 0; i < ruleBlock->numberOfRules(); ++i) {
            ss << tab << toString(ruleBlock->getRule(i)) << _separator;
        }
        return ss.str();
    }

    std::string FllExporter::toString(const Rule* rule) const {
        return "rule: " + rule->getText();
    }

    std::string FllExporter::toString(const Term* term) const {
        return "term: " + term->getName() + " " + term->className()
                + " " + term->parameters();
    }

    std::string FllExporter::toString(const Norm* norm) const {
        if (norm) return norm->className();
        return "none";
    }

    std::string FllExporter::toString(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) return "none";
        const IntegralDefuzzifier* integralDefuzzifier =
                dynamic_cast<const IntegralDefuzzifier*> (defuzzifier);
        if (integralDefuzzifier) {
            return defuzzifier->className() + " " + Op::str<int>(integralDefuzzifier->getResolution());
        }
        return defuzzifier->className();
    }

}
