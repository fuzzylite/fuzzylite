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

    std::string FisExporter::toString(const Engine* engine) const {
        std::ostringstream fis;
        fis << exportSystem(engine) << "\n";

        fis << exportInputs(engine) << "\n";

        fis << exportOutputs(engine) << "\n";

        fis << exportRules(engine) << "\n";

        return fis.str();
    }

    std::string FisExporter::exportSystem(const Engine* engine) const {
        std::ostringstream fis;
        fis << "[System]\n";
        fis << "Name='" << engine->getName() << "'\n";
        fis << "Type='mamdani'\n";
        fis << "Version=" << FL_VERSION << "\n";
        fis << "NumInputs=" << engine->numberOfInputVariables() << "\n";
        fis << "NumOutputs=" << engine->numberOfOutputVariables() << "\n";
        fis << "NumRules=" << engine->getRuleBlock(0)->numberOfRules() << "\n";

        const TNorm* tnorm = NULL;
        const SNorm* snorm = NULL;
        const TNorm* activation = NULL;
        std::string nullptrError, uniquenessError;
        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* rb = engine->getRuleBlock(i);
            if (not rb->getTnorm()) nullptrError = "T-Norm";
            if (not rb->getSnorm()) nullptrError = "S-Norm";
            if (not rb->getActivation()) nullptrError = "activation T-Norm";
            if (not nullptrError.empty()) break;

            if (not tnorm) tnorm = rb->getTnorm();
            else if (tnorm->className() != rb->getTnorm()->className())
                uniquenessError = "T-Norm";

            if (not snorm) snorm = rb->getSnorm();
            else if (snorm->className() != rb->getSnorm()->className())
                uniquenessError = "S-Norm";

            if (not activation) activation = rb->getActivation();
            else if (activation->className() != rb->getActivation()->className())
                uniquenessError = "activation T-Norm";
            if (not uniquenessError.empty()) break;
        }

        if (not nullptrError.empty())
            throw fl::Exception("[nullptr error] expected a " + nullptrError +
                ", but none was set");

        if (not uniquenessError.empty())
            throw fl::Exception("[format error] fis files require all ruleblocks "
                "to have the same " + uniquenessError);

        fis << "AndMethod='" << toFis(tnorm) << "'\n";
        fis << "OrMethod='" << toFis(snorm) << "'\n";
        fis << "ImpMethod='" << toFis(activation) << "'\n";

        const SNorm* accumulation = NULL;
        Defuzzifier* defuzzifier = NULL;
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            if (not outputVariable->getDefuzzifier()) nullptrError = "defuzzifier";
            if (not outputVariable->output()->getAccumulation()) nullptrError = "accumulation S-Norm";
            if (not nullptrError.empty()) break;

            if (not defuzzifier) defuzzifier = outputVariable->getDefuzzifier();
            else if (defuzzifier->className() != outputVariable->getDefuzzifier()->className())
                uniquenessError = "defuzzifier";
            if (not accumulation) accumulation = outputVariable->output()->getAccumulation();
            else if (accumulation->className() != outputVariable->output()->getAccumulation()->className())
                uniquenessError = "accumulation S-Norm";
            if (not uniquenessError.empty()) break;
        }

        if (not nullptrError.empty())
            throw fl::Exception("[nullptr error] expected a " + nullptrError +
                ", but none was set");

        if (not uniquenessError.empty())
            throw fl::Exception("[format error] fis files require all ruleblocks "
                "to have the same " + uniquenessError);

        fis << "AggMethod='" << toFis(accumulation) << "'\n";
        fis << "DefuzzMethod='" << toFis(defuzzifier) << "'\n";
        return fis.str();
    }

    std::string FisExporter::exportInputs(const Engine* engine) const {
        std::ostringstream fis;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* var = engine->getInputVariable(i);
            fis << "[Input" << (i + 1) << "]\n";
            fis << "Name='" << var->getName() << "'\n";
            fis << "Range=[" << var->getMinimum() << " " << var->getMaximum() << "]\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int t = 0; t < var->numberOfTerms(); ++t) {
                fis << "MF" << (t + 1) << "='" << var->getTerm(t)->getName() << "':"
                        << toFis(var->getTerm(t)) << "\n";
            }
        }
        fis << "\n";
        return fis.str();
    }

    std::string FisExporter::exportOutputs(const Engine* engine) const{
        std::ostringstream fis;
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* var = engine->getOutputVariable(i);
            fis << "[Output" << (i + 1) << "]\n";
            fis << "Name='" << var->getName() << "'\n";
            fis << "Range=[" << var->getMinimum() << " " << var->getMaximum() << "]\n";
            fis << "Default=" << var->getDefaultValue() << "\n";
            fis << "Lock=" << var->lockDefuzzifiedValue() << "\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int t = 0; t < var->numberOfTerms(); ++t) {
                fis << "MF" << (t + 1) << "='" << var->getTerm(t)->getName() << "':"
                        << toFis(var->getTerm(t)) << "\n";
            }
        }
        fis << "\n";
        return fis.str();
    }
    
    std::string FisExporter::exportRules(const Engine* engine) const{
        std::ostringstream fis;
        fis << "[Rules]\n";
        return fis.str();
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
        if (defuzzifier->className() == CenterOfGravity().className()) return "centroid";
        if (defuzzifier->className() == SmallestOfMaximum().className()) return "som";
        if (defuzzifier->className() == LargestOfMaximum().className()) return "lom";
        if (defuzzifier->className() == MeanOfMaximum().className()) return "mom";
        return defuzzifier->className();
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