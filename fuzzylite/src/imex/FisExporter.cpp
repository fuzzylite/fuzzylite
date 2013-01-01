/* 
 * File:   FisExporter.cpp
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:53 PM
 */

#include "fl/imex/FisExporter.h"

#include "fl/Headers.h"

namespace fl {

    FisExporter::FisExporter() { }

    FisExporter::~FisExporter() { }

    std::string FisExporter::name() const {
        return "FisExporter";
    }

    std::string FisExporter::toString(const Engine* engine) {
        Configuration* configuration = engine->getConfiguration();
        if (not configuration) {
            throw fl::Exception("[engine error] a configuration object is required within "
                    "the engine to export to FIS format");
        }
        std::ostringstream fcl;
        fcl << "[System]\n";
        fcl << "Name='" << engine->getName() << "'\n";
        fcl << "Type='mamdani'\n";
        fcl << "Version=" << FL_VERSION << "\n";
        fcl << "NumInputs=" << engine->numberOfInputVariables() << "\n";
        fcl << "NumOutputs=" << engine->numberOfOutputVariables() << "\n";
        fcl << "NumRules=" << engine->getRuleBlock(0)->numberOfRules() << "\n";

        fcl << "AndMethod='" << toFis(configuration->getTnorm()) << "\n";
        fcl << "OrMethod='" << toFis(configuration->getSnorm()) << "\n";
        fcl << "ImpMethod='" << toFis(configuration->getActivation()) << "\n";
        fcl << "AggMethod='" << toFis(configuration->getAccumulation()) << "\n";
        fcl << "DefuzzMethod='" << toFis(configuration->getDefuzzifier()) << "\n";

        fcl << "\n";

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* var = engine->getInputVariable(i);
            fcl << "[Input" << (i + 1) << "]\n";
            fcl << "Name='" << var->getName() << "'\n";
            fcl << "Range=[" << var->getMinimum() << " " << var->getMaximum() << "]\n";
            fcl << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int t = 0; t < var->numberOfTerms(); ++t) {
                fcl << "MF" << (t + 1) << "='" << var->getTerm(t)->getName() << "':"
                        << toFis(var->getTerm(t)) << "\n";
            }
        }
        fcl << "\n";

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* var = engine->getOutputVariable(i);
            fcl << "[Output" << (i + 1) << "]\n";
            fcl << "Name='" << var->getName() << "'\n";
            fcl << "Range=[" << var->getMinimum() << " " << var->getMaximum() << "]\n";
            fcl << "Default=" << var->getDefaultValue() << "\n";
            fcl << "Lock=" << var->lockDefuzzifiedValue() << "\n";
            fcl << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int t = 0; t < var->numberOfTerms(); ++t) {
                fcl << "MF" << (t + 1) << "='" << var->getTerm(t)->getName() << "':"
                        << toFis(var->getTerm(t)) << "\n";
            }
        }
        fcl << "\n";

        fcl << "[Rules]\n";

        return fcl.str();
    }

    std::string FisExporter::toFis(const TNorm* tnorm) const {
        if (not tnorm) return "";
        std::string name = tnorm->className();
        if (name == Minimum().className()) return "min";
        if (name == AlgebraicProduct().className()) return "algebraic_product";
        if (name == BoundedDifference().className()) return "bounded_difference";
        if (name == DrasticProduct().className()) return "drastic_product";
        if (name == EinsteinProduct().className()) return "einstein_product";
        if (name == HamacherProduct().className()) return "hamacher_product";
        return tnorm->className();
    }

    std::string FisExporter::toFis(const SNorm * snorm) const {
        if (not snorm) return "";
        std::string name = snorm->className();
        if (name == Maximum().className()) return "max";
        if (name == AlgebraicSum().className()) return "algebraic_sum";
        if (name == BoundedSum().className()) return "bounded_sum";
        if (name == NormalizedSum().className()) return "normalized_sum";
        if (name == DrasticSum().className()) return "drastic_sum";
        if (name == EinsteinSum().className()) return "einstein_sum";
        if (name == HamacherSum().className()) return "hamacher_sum";
        return snorm->className();
    }

    std::string FisExporter::toFis(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) return "";
        if (defuzzifier->name() == CenterOfGravity().name()) return "centroid";
        if (defuzzifier->name() == SmallestOfMaximum().name()) return "som";
        if (defuzzifier->name() == LargestOfMaximum().name()) return "lom";
        if (defuzzifier->name() == MeanOfMaximum().name()) return "mom";
        return defuzzifier->name();
    }

    std::string FisExporter::toFis(const Term* term) const {
        std::ostringstream ss;
        if (term->className() == Bell().className()) {
            const Bell* x = dynamic_cast<const Bell*> (term);
            ss << "'gbellmf',[" << x->getWidth() << " " << x->getSlope() << " "
                    << x->getCenter() << "]";
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
            ss << "'gaussmf',[" << x->getSigma() << " " << x->getMean() << "]";
            return ss.str();
        }

        if (term->className() == GaussianProduct().className()) {
            const GaussianProduct* x = dynamic_cast<const GaussianProduct*> (term);
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
}