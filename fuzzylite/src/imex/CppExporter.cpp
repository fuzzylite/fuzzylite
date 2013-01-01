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
            cpp << "engine->addHedge(new " << toCpp(engine->getHedge(i)) << ");\n";
        }
        cpp << "\n";

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* input = engine->getInputVariable(i);
            cpp << "fl::InputVariable* inputVariable" << (i + 1) << " = new fl::InputVariable;\n";
            cpp << "inputVariable" << (i + 1) << "->setName(\"" << input->getName() << "\");\n";
            cpp << "inputVariable" << (i + 1) << "->setMinimum(" << input->getMinimum() << ");\n";
            cpp << "inputVariable" << (i + 1) << "->setMaximum(" << input->getMaximum() << ");\n";
            for (int t = 0; t < input->numberOfTerms(); ++t) {
                cpp << "inputVariable" << (i + 1) << "->addTerm(new fl::" <<
                        toCpp(input->getTerm(t)) << ");\n";
            }
            cpp << "\n";
        }

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* output = engine->getOutputVariable(i);
            cpp << "fl::OutputVariable* outputVariable" << (i + 1) << " = new fl::OutputVariable;\n";
            cpp << "outputVariable" << (i + 1) << "->setName(\"" << output->getName() << "\");\n";
            cpp << "outputVariable" << (i + 1) << "->setMinimum(" << output->getMinimum() << ");\n";
            cpp << "outputVariable" << (i + 1) << "->setMaximum(" << output->getMaximum() << ");\n";

            cpp << "outputVariable" << (i + 1) << "->setDefaultValue(";
            scalar defaultValue = output->getDefaultValue();
            if (fl::Op::IsNan(defaultValue))
                cpp << "std::numeric_limits<scalar>::quiet_NaN()";
            else if (fl::Op::IsInf(defaultValue))
                cpp << (defaultValue < 0 ? "-" : "") << "std::numeric_limits<scalar>::infinity()";
            else cpp << defaultValue;
            cpp << ");\n";

            cpp << "outputVariable" << (i + 1) << "->setLockDefuzzifiedValue(" <<
                    (output->lockDefuzzifiedValue() ? "true" : "false") << ");\n";

            cpp << "outputVariable" << (i + 1) << "->setDefuzzifier(new fl::" <<
                    output->getDefuzzifier()->className() <<
                    "(" << output->getDefuzzifier()->getDivisions() << "));\n";
            cpp << "outputVariable" << (i + 1) << "->output()->setAccumulation(new fl::" <<
                    output->output()->getAccumulation()->className() << ");\n";

            for (int t = 0; t < output->numberOfTerms(); ++t) {
                cpp << "outputVariable" << (i + 1) << "->addTerm(new fl::" <<
                        toCpp(output->getTerm(t)) << ");\n";
            }
            cpp << "\n";
        }

        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* ruleblock = engine->getRuleBlock(i);
            cpp << "fl::RuleBlock* ruleblock" << (i + 1) << " = new fl::RuleBlock;\n";
            cpp << "ruleblock->setName(\"" << ruleblock->getName() << "\");\n";
            cpp << "ruleblock->setTnorm(new fl::" << ruleblock->getTnorm()->className() << ");\n";
            cpp << "ruleblock->setSnorm(new fl::" << ruleblock->getSnorm()->className() << ");\n";
            cpp << "ruleblock->setActivation(new fl::" << ruleblock->getActivation()->className() << ");\n";

            for (int r = 0; r < ruleblock->numberOfRules(); ++r) {
                cpp << "ruleblock->addRule(fl::MamdaniRule::parse(\"" <<
                        ruleblock->getRule(r)->getUnparsedRule() << "\", engine));\n";
            }
        }

        return cpp.str();
    }

    std::string CppExporter::toCpp(const Term* term) const { 
        std::ostringstream ss;
        throw fl::Exception("Missing imoplementation of discrete");
        ss << term->className() << "(\"" << term->getName() << "\", ";
        ss << std::setprecision(5);
        if (term->className() == Bell().className()) {
            const Bell* x = dynamic_cast<const Bell*> (term);
            ss << x->getCenter() << ", " << x->getWidth() << ", " << x->getSlope()
                    << ")";
            return ss.str();
        }

        if (term->className() == Discrete().className()) {
            ss << "'discretemf',[";
            const Discrete* x = dynamic_cast<const Discrete*> (term);
            for (std::size_t i = 0; i < x->x.size(); ++i) {
                ss << x->x[i] << " " << x->y[i];
                if (i < x->x.size() - 1) ss << " ";
            }
            ss << "]";
            return ss.str();
        }

        if (term->className() == Gaussian().className()) {
            const Gaussian* x = dynamic_cast<const Gaussian*> (term);
            ss << x->getMean() << ", " << x->getSigma() << ")";
            return ss.str();
        }

        if (term->className() == GaussianProduct().className()) {
            const GaussianProduct* x = dynamic_cast<const GaussianProduct*> (term);
            ss << x->getMeanA() << ", " << x->getSigmaA() << ", "
                    << 
            ss << "'gauss2mf',[" << x->getSigmaA() << " " << x->getMeanA() <<
                    x->getSigmaB() << " " << x->getMeanB() << "]";
            return ss.str();
        }

        if (term->className() == PiShape().className()) {
            const PiShape* x = dynamic_cast<const PiShape*> (term);
            ss << "'pimf',[" << x->getA() << " " << x->getB() << " "
                    << x->getC() << " " << x->getD() << "]";
            return ss.str();
        }

        if (term->className() == Ramp().className()) {
            const Ramp* x = dynamic_cast<const Ramp*> (term);
            ss << "'rampmf',[" << x->getStart() << " " << x->getEnd() << "]";
            return ss.str();
        }

        if (term->className() == Rectangle().className()) {
            const Rectangle* x = dynamic_cast<const Rectangle*> (term);
            ss << "'rectmf',[" << x->getMinimum() << " " << x->getMaximum() << "]";
            return ss.str();
        }

        if (term->className() == SShape().className()) {
            const SShape* x = dynamic_cast<const SShape*> (term);
            ss << "'smf',[" << x->getStart() << " " << x->getEnd() << "]";
            return ss.str();
        }

        if (term->className() == Sigmoid().className()) {
            const Sigmoid* x = dynamic_cast<const Sigmoid*> (term);
            ss << "'sigmf',[" << x->getSlope() << " " << x->getInflection() << "]";
            return ss.str();
        }

        if (term->className() == SigmoidDifference().className()) {
            const SigmoidDifference* x = dynamic_cast<const SigmoidDifference*> (term);
            ss << "'dsigmf',[" << x->getRisingSlope() << " " << x->getLeftInflection() <<
                    " " << x->getFallingSlope() << " " << x->getRightInflection() << "]";
            return ss.str();
        }

        if (term->className() == SigmoidProduct().className()) {
            const SigmoidProduct* x = dynamic_cast<const SigmoidProduct*> (term);
            ss << "'psigmf',[" << x->getRisingSlope() << " " << x->getLeftInflection() <<
                    " " << x->getFallingSlope() << " " << x->getRightInflection() << "]";
            return ss.str();
        }

        if (term->className() == Trapezoid().className()) {
            const Trapezoid* x = dynamic_cast<const Trapezoid*> (term);
            ss << "'trapmf',[" << x->getA() << " " << x->getB() << " " <<
                    x->getC() << " " << x->getD() << "]";
            return ss.str();
        }

        if (term->className() == Triangle().className()) {
            const Triangle* x = dynamic_cast<const Triangle*> (term);
            ss << "'trimf',[" << x->getA() << " " << x->getB() << " " << x->getC() << "]";
            return ss.str();
        }

        if (term->className() == ZShape().className()) {
            const ZShape* x = dynamic_cast<const ZShape*> (term);
            ss << "'zmf',[" << x->getStart() << " " << x->getEnd() << "]";
            return ss.str();
        }

        ss << "[term error] term of class <" << term->className() << "> not supported";
        throw fl::Exception(ss.str());
    }

    std::string CppExporter::toCpp(const Hedge* hedge) const {
        if (hedge->name() == Any().name()) return "Any";
        if (hedge->name() == Extremely().name()) return "Extremely";
        if (hedge->name() == Not().name()) return "Not";
        if (hedge->name() == Seldom().name()) return "Seldom";
        if (hedge->name() == Somewhat().name()) return "Somewhat";
        if (hedge->name() == Very().name()) return "Very";
        return hedge->name();
    }

}
