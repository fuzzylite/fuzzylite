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
 * File:   JavaExporter.cpp
 * Author: jcrada
 *
 * Created on 28 November 2013, 10:21 PM
 */

#include "fl/imex/JavaExporter.h"

#include "fl/Headers.h"

#include <algorithm>
namespace fl {

    JavaExporter::JavaExporter() : Exporter() {
    }

    JavaExporter::~JavaExporter() {

    }

    std::string JavaExporter::name() const {
        return "JavaExporter";
    }

    std::string JavaExporter::toString(const Engine* engine) const {
        std::ostringstream ss;
        ss << "Engine engine = new Engine();\n";
        ss << "engine.setName(\"" << engine->getName() << "\");\n";
        ss << "\n";

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            ss << toString(engine->getInputVariable(i), engine) << "\n";
        }

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            ss << toString(engine->getOutputVariable(i), engine) << "\n";
        }

        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            ss << toString(engine->getRuleBlock(i), engine) << "\n";
        }

        return ss.str();
    }

    std::string JavaExporter::toString(const InputVariable* inputVariable, const Engine* engine) const {
        std::ostringstream ss;
        std::string name = "inputVariable";
        if (engine->numberOfInputVariables() > 1) {
            int index = std::distance(engine->inputVariables().begin(),
                    std::find(engine->inputVariables().begin(),
                    engine->inputVariables().end(), inputVariable));
            name += Op::str<int>(index + 1);
        }
        ss << "InputVariable " << name << " = new InputVariable();\n";
        ss << name << ".setEnabled(" << (inputVariable->isEnabled() ? "true" : "false") << ");\n";
        ss << name << ".setName(\"" << inputVariable->getName() << "\");\n";
        ss << name << ".setRange("
                << toString(inputVariable->getMinimum()) << ", "
                << toString(inputVariable->getMaximum()) << ");\n";

        for (int i = 0; i < inputVariable->numberOfTerms(); ++i) {
            ss << name << ".addTerm(" <<
                    toString(inputVariable->getTerm(i)) << ");\n";
        }
        ss << "engine.addInputVariable(" << name << ");\n";
        return ss.str();
    }

    std::string JavaExporter::toString(const OutputVariable* outputVariable, const Engine* engine) const {
        std::ostringstream ss;
        std::string name = "outputVariable";
        if (engine->numberOfOutputVariables() > 1) {
            int index = std::distance(engine->outputVariables().begin(),
                    std::find(engine->outputVariables().begin(),
                    engine->outputVariables().end(), outputVariable));
            name += Op::str<int>(index + 1);
        }
        ss << "OutputVariable " << name << " = new OutputVariable();\n";
        ss << name << ".setEnabled(" << (outputVariable->isEnabled() ? "true" : "false") << ");\n";
        ss << name << ".setName(\"" << outputVariable->getName() << "\");\n";
        ss << name << ".setRange("
                << toString(outputVariable->getMinimum()) << ", "
                << toString(outputVariable->getMaximum()) << ");\n";
        ss << name << ".fuzzyOutput().setAccumulation(" <<
                toString(outputVariable->fuzzyOutput()->getAccumulation()) << ");\n";
        ss << name << ".setDefuzzifier(" <<
                toString(outputVariable->getDefuzzifier()) << ");\n";
        ss << name << ".setDefaultValue(" <<
                toString(outputVariable->getDefaultValue()) << ");\n";
        ss << name << ".setLockValidOutput(" <<
                (outputVariable->isLockingValidOutput() ? "true" : "false") << ");\n";
        ss << name << ".setLockOutputRange(" <<
                (outputVariable->isLockingOutputRange() ? "true" : "false") << ");\n";
        for (int i = 0; i < outputVariable->numberOfTerms(); ++i) {
            ss << name << ".addTerm(" <<
                    toString(outputVariable->getTerm(i)) << ");\n";
        }
        ss << "engine.addOutputVariable(" << name << ");\n";
        return ss.str();
    }

    std::string JavaExporter::toString(const RuleBlock* ruleBlock, const Engine* engine) const {
        std::ostringstream ss;
        std::string name = "ruleBlock";
        if (engine->numberOfRuleBlocks() > 1) {
            int index = std::distance(engine->ruleBlocks().begin(),
                    std::find(engine->ruleBlocks().begin(),
                    engine->ruleBlocks().end(), ruleBlock));
            name += Op::str<int>(index + 1);
        }
        ss << "RuleBlock " << name << " = new RuleBlock();\n";
        ss << name << ".setEnabled(" << (ruleBlock->isEnabled() ? "true" : "false") << ");\n";
        ss << name << ".setName(\"" << ruleBlock->getName() << "\");\n";
        ss << name << ".setConjunction("
                << toString(ruleBlock->getConjunction()) << ");\n";
        ss << name << ".setDisjunction("
                << toString(ruleBlock->getDisjunction()) << ");\n";
        ss << name << ".setActivation("
                << toString(ruleBlock->getActivation()) << ");\n";
        for (int i = 0; i < ruleBlock->numberOfRules(); ++i) {
            Rule* rule = ruleBlock->getRule(i);
            ss << name << ".addRule(Rule.parse(\"" << rule->getText() << "\", engine));\n";
        }
        ss << "engine.addRuleBlock(" << name << ");\n";
        return ss.str();
    }

    std::string JavaExporter::toString(const Term* term) const {
        if (not term) {
            return "null";
        }

        if (term->className() == Discrete().className()) {
            const Discrete* discrete = dynamic_cast<const Discrete*> (term);
            std::ostringstream ss;
            std::vector<scalar> xy;
            for (std::size_t i = 0; i < discrete->x.size(); ++i) {
                xy.push_back(discrete->x.at(i));
                xy.push_back(discrete->y.at(i));
            }
            ss << term->className() << ".create(\"" << term->getName() << "\", "
                    << Op::join(xy, ", ") << ")";
            return ss.str();
        }


        if (term->className() == Function().className()) {
            const Function* function = dynamic_cast<const Function*> (term);
            std::ostringstream ss;
            ss << term->className() << ".create(\"" << term->getName() << "\", "
                    << "\"" << function->getFormula() << "\", engine, true)";
            return ss.str();
        }

        if (term->className() == Linear().className()) {
            const Linear* linear = dynamic_cast<const Linear*> (term);
            std::ostringstream ss;
            ss << term->className() << ".create(\"" << term->getName() << "\", "
                    << "engine.getInputVariables(), "
                    << Op::join(linear->coefficients, ", ") << ")";
            return ss.str();
        }

        std::ostringstream ss;
        ss << "new " << term->className() << "(\"" << term->getName() << "\", "
                << Op::findReplace(term->parameters(), " ", ", ") << ")";
        return ss.str();
    }

    std::string JavaExporter::toString(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) {
            return "null";
        }
        const IntegralDefuzzifier* integralDefuzzifier =
                dynamic_cast<const IntegralDefuzzifier*> (defuzzifier);
        if (integralDefuzzifier) {
            std::ostringstream ss;
            ss << "new " << defuzzifier->className() << "(" <<
                    integralDefuzzifier->getResolution() << ")";
            return ss.str();
        }
        return "new " + defuzzifier->className() + "()";
    }

    std::string JavaExporter::toString(const Norm* norm) const {
        if (not norm) {
            return "null";
        }
        return "new " + norm->className() + "()";
    }

    std::string JavaExporter::toString(scalar value) const {
        if (Op::isNan(value)) {
            return "Double.NaN";
        } else if (Op::isInf(value)) {
            return (value > 0
                    ? "Double.POSITIVE_INFINITY"
                    : "Double.NEGATIVE_INFINITY");
        }
        return Op::str(value);
    }

}

