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

    FllExporter::FllExporter(const std::string& indent) : Exporter(),
    _indent(indent) {
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

    std::string FllExporter::toString(const Engine* engine) const {
        std::ostringstream ss;
        ss << "Engine: " << engine->getName() << "\n\n";
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            ss << toString(engine->getInputVariable(i)) << "\n";
        }
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            ss << toString(engine->getOutputVariable(i)) << "\n";
        }
        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            ss << toString(engine->getRuleBlock(i)) << "\n";
        }
        return ss.str();
    }

    std::string FllExporter::toString(const Variable* variable) const {
        std::ostringstream ss;
        ss << "Variable: " << variable->getName() << "\n";
        std::string tab = _indent;
        ss << tab << "enabled: " << (variable->isEnabled() ? "true" : "false") << "\n";
        ss << tab << "range: " << Op::str(variable->getMinimum())
                << " " << Op::str(variable->getMaximum()) << "\n";
        for (int i = 0; i < variable->numberOfTerms(); ++i) {
            ss << tab << toString(variable->getTerm(i)) << "\n";
        }
        return ss.str();
    }

    std::string FllExporter::toString(const InputVariable* inputVariable) const {
        std::ostringstream ss;
        ss << "InputVariable: " << inputVariable->getName() << "\n";
        std::string tab = _indent;
        ss << tab << "enabled: " << (inputVariable->isEnabled() ? "true" : "false") << "\n";
        ss << tab << "range: " << Op::str(inputVariable->getMinimum())
                << " " << Op::str(inputVariable->getMaximum()) << "\n";
        for (int i = 0; i < inputVariable->numberOfTerms(); ++i) {
            ss << tab << toString(inputVariable->getTerm(i)) << "\n";
        }
        return ss.str();
    }

    std::string FllExporter::toString(const OutputVariable* outputVariable) const {
        std::ostringstream ss;
        ss << "OutputVariable: " << outputVariable->getName() << "\n";
        std::string tab = _indent;
        ss << tab << "enabled: " << (outputVariable->isEnabled() ? "true" : "false") << "\n";
        ss << tab << "range: " << Op::str(outputVariable->getMinimum())
                << " " << Op::str(outputVariable->getMaximum()) << "\n";
        ss << tab << "default: " << Op::str(outputVariable->getDefaultValue()) << "\n";
        ss << tab << "lock-valid: " << (outputVariable->isLockingValidOutput() ? "true" : "false") << "\n";
        ss << tab << "lock-range: " << (outputVariable->isLockingOutputRange() ? "true" : "false") << "\n";
        ss << tab << "defuzzifier: " << toString(outputVariable->getDefuzzifier()) << "\n";
        ss << tab << "accumulation: " << toString(outputVariable->output()->getAccumulation()) << "\n";
        for (int i = 0; i < outputVariable->numberOfTerms(); ++i) {
            ss << tab << toString(outputVariable->getTerm(i)) << "\n";
        }
        return ss.str();
    }

    std::string FllExporter::toString(const RuleBlock* ruleBlock) const {
        std::ostringstream ss;
        ss << "RuleBlock: " << ruleBlock->getName() << "\n";
        std::string tab = _indent;
        ss << tab << "enabled: " << (ruleBlock->isEnabled() ? "true" : "false") << "\n";
        ss << tab << "conjunction: " << toString(ruleBlock->getConjunction()) << "\n";
        ss << tab << "disjunction: " << toString(ruleBlock->getDisjunction()) << "\n";
        ss << tab << "activation: " << toString(ruleBlock->getActivation()) << "\n";
        for (int i = 0; i < ruleBlock->numberOfRules(); ++i) {
            ss << tab << toString(ruleBlock->getRule(i)) << "\n";
        }
        return ss.str();
    }

    std::string FllExporter::toString(const Rule* rule) const {
        std::ostringstream ss;
        ss << "Rule: ";
        if (rule) ss << rule->getText();
        else ss << "none";
        return ss.str();
    }

    std::string FllExporter::toString(const Term* term) const {
        std::ostringstream ss;
        ss << "Term: ";
        if (term) {
            ss << term->getName() << " = " << term->className() << " "
                    << term->parameters();
        } else {
            ss << "none";
        }
        return ss.str();
    }

    std::string FllExporter::toString(const Norm* norm) const {
        if (norm) return norm->className();
        return "none";
    }

    std::string FllExporter::toString(const Defuzzifier* defuzzifier) const {
        std::ostringstream ss;
        if (defuzzifier) {
            ss << defuzzifier->className();
            const IntegralDefuzzifier* integralDefuzzifier =
                    dynamic_cast<const IntegralDefuzzifier*> (defuzzifier);
            if (integralDefuzzifier) {
                ss << " " << integralDefuzzifier->getResolution();
            }
        } else {
            ss << "none";
        }
        return ss.str();
    }

}
