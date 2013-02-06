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
 * File:   CppExporter.cpp
 * Author: jcrada
 * 
 * Created on 1 January 2013, 2:41 PM
 */

#include "fl/imex/CppExporter.h"

#include "fl/Headers.h"

namespace fl {

    CppExporter::CppExporter() { }

    CppExporter::~CppExporter() { }

    std::string CppExporter::name() const {
        return "CppExporter";
    }

    std::string CppExporter::toString(const Engine* engine) const {
        std::ostringstream cpp;
        cpp << "fl::Engine* engine = new fl::Engine;\n";
        cpp << "engine->setName(\"" << engine->getName() << "\");\n";

        for (int i = 0; i < engine->numberOfHedges(); ++i) {
            cpp << "engine->addHedge(" << toCpp(engine->getHedge(i)) << ");\n";
        }
        cpp << "\n";

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* input = engine->getInputVariable(i);
            cpp << "fl::InputVariable* inputVariable" << (i + 1) << " = new fl::InputVariable;\n";
            cpp << "inputVariable" << (i + 1) << "->setName(\"" << input->getName() << "\");\n";
            cpp << "inputVariable" << (i + 1) << "->setRange(" <<
                    fl::Op::str(input->getMinimum()) << ", " <<
                    fl::Op::str(input->getMaximum()) << ");\n";
            cpp << "\n";
            for (int t = 0; t < input->numberOfTerms(); ++t) {
                cpp << "inputVariable" << (i + 1) << "->addTerm(" <<
                        toCpp(input->getTerm(t)) << ");\n";
            }
            cpp << "engine->addInputVariable(inputVariable" << (i + 1) << ");\n";
            cpp << "\n";
        }

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* output = engine->getOutputVariable(i);
            cpp << "fl::OutputVariable* outputVariable" << (i + 1) << " = new fl::OutputVariable;\n";
            cpp << "outputVariable" << (i + 1) << "->setName(\"" << output->getName() << "\");\n";
            cpp << "outputVariable" << (i + 1) << "->setRange(" <<
                    fl::Op::str(output->getMinimum()) << ", " <<
                    fl::Op::str(output->getMaximum()) << ");\n";

            cpp << "outputVariable" << (i + 1) << "->setDefaultValue(";
            scalar defaultValue = output->getDefaultValue();
            if (fl::Op::isNan(defaultValue))
                cpp << "std::numeric_limits<fl::scalar>::quiet_NaN()";
            else if (fl::Op::isInf(defaultValue))
                cpp << (defaultValue < 0 ? "-" : "") << "std::numeric_limits<scalar>::infinity()";
            else cpp << fl::Op::str(defaultValue);
            cpp << ");\n";

            cpp << "outputVariable" << (i + 1) << "->setLockDefuzzifiedValue(" <<
                    (output->lockDefuzzifiedValue() ? "true" : "false") << ");\n";

            cpp << "outputVariable" << (i + 1) << "->setDefuzzifier(" <<
                    toCpp(output->getDefuzzifier()) << ");\n";
            cpp << "outputVariable" << (i + 1) << "->output()->setAccumulation(" <<
                    toCpp(output->output()->getAccumulation()) << ");\n";
            cpp << "\n";
            for (int t = 0; t < output->numberOfTerms(); ++t) {
                cpp << "outputVariable" << (i + 1) << "->addTerm(" <<
                        toCpp(output->getTerm(t)) << ");\n";
            }
            cpp << "engine->addOutputVariable(outputVariable" << (i + 1) << ");\n";
            cpp << "\n";
        }

        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* ruleblock = engine->getRuleBlock(i);
            cpp << "fl::RuleBlock* ruleblock" << (i + 1) << " = new fl::RuleBlock;\n";
            cpp << "ruleblock" << (i + 1) << "->setName(\"" << ruleblock->getName() << "\");\n";
            cpp << "ruleblock" << (i + 1) << "->setTnorm(" <<
                    toCpp(ruleblock->getTnorm()) << ");\n";
            cpp << "ruleblock" << (i + 1) << "->setSnorm("
                    << toCpp(ruleblock->getSnorm()) << ");\n";
            cpp << "ruleblock" << (i + 1) << "->setActivation("
                    << toCpp(ruleblock->getActivation()) << ");\n";
            cpp << "\n";
            for (int r = 0; r < ruleblock->numberOfRules(); ++r) {
                cpp << "ruleblock" << (i + 1) << "->addRule(fl::MamdaniRule::parse(\n\t\"" <<
                        ruleblock->getRule(r)->getUnparsedRule() << "\", engine));\n";
            }
            cpp << "engine->addRuleBlock(ruleblock" << (i + 1) << ");\n";
        }
        cpp << "\n";

        return cpp.str();
    }

    std::string CppExporter::toCpp(const Term* term) const {
        if (not term) return "NULL";
        std::ostringstream ss;
        ss << "new fl::" << term->className() << "(\"" << term->getName() << "\", ";

        if (term->className() == Bell().className()) {
            const Bell* x = dynamic_cast<const Bell*> (term);
            scalar params[] = {x->getCenter(), x->getWidth(), x->getSlope()};
            ss << fl::Op::str(3, params) << ")";
            return ss.str();
        }

        if (term->className() == Discrete().className()) {
            const Discrete* x = dynamic_cast<const Discrete*> (term);
            ss << x->x.size() + x->y.size() << ",";
            for (std::size_t i = 0; i < x->x.size(); ++i) {
                ss << fl::Op::str(x->x.at(i)) << "," << fl::Op::str(x->y.at(i));
                if (i < x->x.size() - 1) ss << ",";
            }
            ss << ")";
            return ss.str();
        }

        if (term->className() == Gaussian().className()) {
            const Gaussian* x = dynamic_cast<const Gaussian*> (term);
            scalar params[] = {x->getMean(), x->getStandardDeviation()};
            ss << fl::Op::str(2, params) << ")";
            return ss.str();
        }

        if (term->className() == GaussianProduct().className()) {
            const GaussianProduct* x = dynamic_cast<const GaussianProduct*> (term);
            scalar params[] = {x->getMeanA(), x->getStandardDeviationA(), x->getMeanB(), x->getStandardDeviationB()};
            ss << fl::Op::str(4, params) << ")";
            return ss.str();
        }

        if (term->className() == PiShape().className()) {
            const PiShape* x = dynamic_cast<const PiShape*> (term);
            scalar params[] = {x->getBottomLeft(), x->getTopLeft(), x->getTopRight(), x->getBottomRight()};
            ss << fl::Op::str(4, params) << ")";
            return ss.str();
        }

        if (term->className() == Ramp().className()) {
            const Ramp* x = dynamic_cast<const Ramp*> (term);
            scalar params[] = {x->getStart(), x->getEnd()};
            ss << fl::Op::str(2, params) << ")";
            return ss.str();
        }

        if (term->className() == Rectangle().className()) {
            const Rectangle* x = dynamic_cast<const Rectangle*> (term);
            scalar params[] = {x->getMinimum(), x->getMaximum()};
            ss << fl::Op::str(2, params) << ")";
            return ss.str();
        }

        if (term->className() == SShape().className()) {
            const SShape* x = dynamic_cast<const SShape*> (term);
            scalar params[] = {x->getStart(), x->getEnd()};
            ss << fl::Op::str(2, params) << ")";
            return ss.str();
        }

        if (term->className() == Sigmoid().className()) {
            const Sigmoid* x = dynamic_cast<const Sigmoid*> (term);
            scalar params[] = {x->getInflection(), x->getSlope()};
            ss << fl::Op::str(2, params) << ")";
            return ss.str();
        }

        if (term->className() == SigmoidDifference().className()) {
            const SigmoidDifference* x = dynamic_cast<const SigmoidDifference*> (term);
            scalar params[] = {x->getLeft(), x->getRising(), x->getFalling(), x->getRight()};
            ss << fl::Op::str(4, params) << ")";
            return ss.str();
        }

        if (term->className() == SigmoidProduct().className()) {
            const SigmoidProduct* x = dynamic_cast<const SigmoidProduct*> (term);
            scalar params[] = {x->getLeft(), x->getRising(), x->getFalling(), x->getRight()};
            ss << fl::Op::str(4, params) << ")";
            return ss.str();
        }

        if (term->className() == Trapezoid().className()) {
            const Trapezoid* x = dynamic_cast<const Trapezoid*> (term);
            scalar params[] = {x->getA(), x->getB(), x->getC(), x->getD()};
            ss << fl::Op::str(4, params) << ")";
            return ss.str();
        }

        if (term->className() == Triangle().className()) {
            const Triangle* x = dynamic_cast<const Triangle*> (term);
            scalar params[] = {x->getA(), x->getB(), x->getC()};
            ss << fl::Op::str(3, params) << ")";
            return ss.str();
        }

        if (term->className() == ZShape().className()) {
            const ZShape* x = dynamic_cast<const ZShape*> (term);
            scalar params[] = {x->getStart(), x->getEnd()};
            ss << fl::Op::str(2, params) << ")";
            return ss.str();
        }

        ss << "[exporter error] term of class <" << term->className() << "> not supported";
        throw fl::Exception(ss.str(), FL_AT);
    }

    std::string CppExporter::toCpp(const Hedge * hedge) const {
        if (hedge->name() == Any().name()) return "new fl::Any";
        if (hedge->name() == Extremely().name()) return "new fl::Extremely";
        if (hedge->name() == Not().name()) return "new fl::Not";
        if (hedge->name() == Seldom().name()) return "new fl::Seldom";
        if (hedge->name() == Somewhat().name()) return "new fl::Somewhat";
        if (hedge->name() == Very().name()) return "new fl::Very";
        return hedge->name();
    }

    std::string CppExporter::toCpp(const Operator* op) const {
        if (not op) return "NULL";
        return "new fl::" + op->className();
    }

    std::string CppExporter::toCpp(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) return "NULL";
        return "new fl::" + defuzzifier->className() + "("
                + fl::Op::str((scalar) defuzzifier->getDivisions(), 0) + ")";
    }

}
