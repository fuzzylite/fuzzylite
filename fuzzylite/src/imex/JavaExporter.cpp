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
        ss << name << ".setName(\"" << outputVariable->getName() << "\");\n";
        ss << name << ".setRange("
                << toString(outputVariable->getMinimum()) << ", "
                << toString(outputVariable->getMaximum()) << ");\n";
        ss << name << ".setLockOutputRange(" <<
                (outputVariable->isLockingOutputRange() ? "true" : "false") << ");\n";
        ss << name << ".setLockValidOutput(" <<
                (outputVariable->isLockingValidOutput() ? "true" : "false") << ");\n";
        ss << name << ".setDefaultValue(" <<
                toString(outputVariable->getDefaultValue()) << ");\n";
        ss << name << ".setDefuzzifier(" <<
                toString(outputVariable->getDefuzzifier()) << ");\n";
        ss << name << ".output().setAccumulation(" <<
                toString(outputVariable->output()->getAccumulation()) << ");\n";

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
        std::ostringstream ss;
        ss << "new " << term->className() << "(\"" << term->getName() << "\", ";
        if (term->className() == Bell().className()) {
            const Bell* x = dynamic_cast<const Bell*> (term);
            ss << fl::Op::join(3, ", ",
                    x->getCenter(), x->getWidth(), x->getSlope()) << ")";
            return ss.str();
        }

        if (term->className() == Constant().className()) {
            const Constant* x = dynamic_cast<const Constant*> (term);
            ss << toString(x->getValue()) << ")";
            return ss.str();
        }

        if (term->className() == Discrete().className()) {
            const Discrete* x = dynamic_cast<const Discrete*> (term);
            std::vector<scalar> xy;
            for (std::size_t i = 0; i < x->x.size(); ++i) {
                xy.push_back(x->x.at(i));
                xy.push_back(x->y.at(i));
            }
            ss.str(std::string()); //clear stream
            ss << term->className() << ".create(\"" << term->getName() << "\", "
                    << Op::join(xy, ", ") << ")";
            return ss.str();
        }

        if (term->className() == Function().className()) {
            const Function* x = dynamic_cast<const Function*> (term);
            ss.str(std::string()); //clear stream
            ss << x->className() << ".create(\"" << x->getName() << "\", "
                    << "\"" << x->getText() << "\", engine, true)";
            return ss.str();
        }

        if (term->className() == Gaussian().className()) {
            const Gaussian* x = dynamic_cast<const Gaussian*> (term);
            ss << fl::Op::join(2, ", ",
                    x->getMean(), x->getStandardDeviation()) << ")";
            return ss.str();
        }

        if (term->className() == GaussianProduct().className()) {
            const GaussianProduct* x = dynamic_cast<const GaussianProduct*> (term);
            ss << fl::Op::join(4, ", ",
                    x->getMeanA(), x->getStandardDeviationA(),
                    x->getMeanB(), x->getStandardDeviationB()) << ")";
            return ss.str();
        }

        if (term->className() == Linear().className()) {
            const Linear* x = dynamic_cast<const Linear*> (term);
            ss.str(std::string()); //clear stream
            ss << x->className() << ".create(\"" << x->getName() << "\", "
                    << "engine.getInputVariables(), "
                    << Op::join(x->coefficients, ", ") << ")";
            return ss.str();
        }

        if (term->className() == PiShape().className()) {
            const PiShape* x = dynamic_cast<const PiShape*> (term);
            ss << fl::Op::join(4, ", ",
                    x->getBottomLeft(), x->getTopLeft(),
                    x->getTopRight(), x->getBottomRight()) << ")";
            return ss.str();
        }

        if (term->className() == Ramp().className()) {
            const Ramp* x = dynamic_cast<const Ramp*> (term);
            ss << fl::Op::join(2, ", ",
                    x->getStart(), x->getEnd()) << ")";
            return ss.str();
        }

        if (term->className() == Rectangle().className()) {
            const Rectangle* x = dynamic_cast<const Rectangle*> (term);
            ss << fl::Op::join(2, ", ",
                    x->getMinimum(), x->getMaximum()) << ")";
            return ss.str();
        }

        if (term->className() == SShape().className()) {
            const SShape* x = dynamic_cast<const SShape*> (term);
            ss << fl::Op::join(2, ", ",
                    x->getStart(), x->getEnd()) << ")";
            return ss.str();
        }

        if (term->className() == Sigmoid().className()) {
            const Sigmoid* x = dynamic_cast<const Sigmoid*> (term);
            ss << fl::Op::join(2, ", ",
                    x->getInflection(), x->getSlope()) << ")";
            return ss.str();
        }

        if (term->className() == SigmoidDifference().className()) {
            const SigmoidDifference* x = dynamic_cast<const SigmoidDifference*> (term);
            ss << fl::Op::join(4, ", ",
                    x->getLeft(), x->getRising(),
                    x->getFalling(), x->getRight()) << ")";
            return ss.str();
        }

        if (term->className() == SigmoidProduct().className()) {
            const SigmoidProduct* x = dynamic_cast<const SigmoidProduct*> (term);
            ss << fl::Op::join(4, ", ",
                    x->getLeft(), x->getRising(),
                    x->getFalling(), x->getRight()) << ")";
            return ss.str();
        }

        if (term->className() == Trapezoid().className()) {
            const Trapezoid* x = dynamic_cast<const Trapezoid*> (term);
            ss << fl::Op::join(4, ", ",
                    x->getA(), x->getB(), x->getC(), x->getD()) << ")";
            return ss.str();
        }

        if (term->className() == Triangle().className()) {
            const Triangle* x = dynamic_cast<const Triangle*> (term);
            ss << fl::Op::join(3, ", ",
                    x->getA(), x->getB(), x->getC()) << ")";
            return ss.str();
        }

        if (term->className() == ZShape().className()) {
            const ZShape* x = dynamic_cast<const ZShape*> (term);
            ss << fl::Op::join(2, ", ",
                    x->getStart(), x->getEnd()) << ")";
            return ss.str();
        }

        ss << "[exporter error] term of class <" << term->className() << "> not supported";
        throw fl::Exception(ss.str(), FL_AT);
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

