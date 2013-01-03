/* 
 * File:   FisExporter.cpp
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:53 PM
 */

#include "fl/imex/FisExporter.h"

#include "fl/Headers.h"
#include <queue>
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
        if (engine->numberOfRuleBlocks() > 0)
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
            throw fl::Exception("[export error] fis files require all ruleblocks "
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
            throw fl::Exception("[export error] fis files require all ruleblocks "
                "to have the same " + uniquenessError);

        fis << "AggMethod='" << toFis(accumulation) << "'\n";
        fis << "DefuzzMethod='" << toFis(defuzzifier) << "'\n";
        return fis.str();
    }

    std::string FisExporter::exportInputs(const Engine* engine) const {
        std::ostringstream fis;
        for (int ixVar = 0; ixVar < engine->numberOfInputVariables(); ++ixVar) {
            InputVariable* var = engine->getInputVariable(ixVar);
            fis << "[Input" << (ixVar + 1) << "]\n";
            fis << "Name='" << var->getName() << "'\n";
            fis << "Range=[" << var->getMinimum() << " " << var->getMaximum() << "]\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int ixTerm = 0; ixTerm < var->numberOfTerms(); ++ixTerm) {
                fis << "MF" << (ixTerm + 1) << "='" << var->getTerm(ixTerm)->getName() << "':"
                        << toFis(var->getTerm(ixTerm)) << "\n";
            }
        }
        fis << "\n";
        return fis.str();
    }

    std::string FisExporter::exportOutputs(const Engine* engine) const {
        std::ostringstream fis;
        for (int ixVar = 0; ixVar < engine->numberOfOutputVariables(); ++ixVar) {
            OutputVariable* var = engine->getOutputVariable(ixVar);
            fis << "[Output" << (ixVar + 1) << "]\n";
            fis << "Name='" << var->getName() << "'\n";
            fis << "Range=[" << var->getMinimum() << " " << var->getMaximum() << "]\n";
            fis << "Default=" << var->getDefaultValue() << "\n";
            fis << "Lock=" << var->lockDefuzzifiedValue() << "\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int ixTerm = 0; ixTerm < var->numberOfTerms(); ++ixTerm) {
                fis << "MF" << (ixTerm + 1) << "='" << var->getTerm(ixTerm)->getName() << "':"
                        << toFis(var->getTerm(ixTerm)) << "\n";
            }
        }
        fis << "\n";
        return fis.str();
    }

    std::string FisExporter::exportRules(const Engine* engine) const {
        std::ostringstream fis;
        fis << "[Rules]\n";
        for (int ixRuleBlock = 0; ixRuleBlock < engine->numberOfRuleBlocks(); ++ixRuleBlock) {
            RuleBlock* rb = engine->getRuleBlock(ixRuleBlock);
            fis << "# RuleBlock " << rb->getName() << "\n";
            for (int ixRule = 0; ixRule < rb->numberOfRules(); ++ixRule) {
                fis << exportRule(dynamic_cast<MamdaniRule*> (rb->getRule(ixRule)), engine) << "\n";
            }
        }
        return fis.str();
    }

    std::string FisExporter::exportRule(const MamdaniRule* rule, const Engine* engine) const {
        if (not rule) return "";
        std::vector<MamdaniProposition*> propositions;
        std::vector<MamdaniOperator*> operators;

        std::queue<MamdaniExpression*> bfsQueue;
        bfsQueue.push(rule->getAntecedent()->getRoot());
        while (not bfsQueue.empty()) {
            MamdaniExpression* front = bfsQueue.front();
            bfsQueue.pop();
            if (front->isOperator) {
                MamdaniOperator* op = dynamic_cast<MamdaniOperator*> (front);
                bfsQueue.push(op->left);
                bfsQueue.push(op->right);
                operators.push_back(op);
            } else {
                propositions.push_back(dynamic_cast<MamdaniProposition*> (front));
            }
        }

        bool equalOperators = true;
        for (int i = 0; i < (int) operators.size() - 1; ++i) {
            if (operators[i]->name != operators[i + 1]->name) {
                equalOperators = false;
                break;
            }
        }
        if (not equalOperators)
            throw fl::Exception("[export error] "
                "fis files do not support rules with different connectors "
                "(i.e. ['and', 'or']). All connectors within a rule must be the same");

        std::ostringstream fis;
        std::vector<Variable*> inputVariables, outputVariables;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i)
            inputVariables.push_back(engine->getInputVariable(i));
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i)
            outputVariables.push_back(engine->getOutputVariable(i));

        fis << translate(propositions, inputVariables) << ", ";
        fis << translate(rule->getConsequent()->conclusions(), outputVariables);
        fis << "(" << rule->getWeight() << ") : ";
        if (operators.size() == 0) fis << "1"; //does not matter
        else {
            if (operators[0]->name == Rule::FL_AND) fis << "1";
            else if (operators[0]->name == Rule::FL_OR) fis << "2";
            else fis << operators[0]->name;
        }
        return fis.str();
    }

    std::string FisExporter::translate(const std::vector<MamdaniProposition*>& propositions,
            const std::vector<Variable*> variables) const {
        std::ostringstream ss;
        for (std::size_t ixVariable = 0; ixVariable < variables.size(); ++ixVariable) {
            Variable* variable = variables[ixVariable];
            int termIndexPlusOne = 0;
            scalar plusHedge = 0.0;
            for (std::size_t ixProposition = 0; ixProposition < propositions.size(); ++ixProposition) {
                MamdaniProposition* proposition = propositions[ixProposition];
                if (proposition->variable != variable) continue;

                for (int termIndex = 0; termIndex < variable->numberOfTerms(); ++termIndex) {
                    if (variable->getTerm(termIndex) == proposition->term) {
                        termIndexPlusOne = termIndex + 1;
                        break;
                    }
                }
                std::vector<Hedge*> hedges = proposition->hedges;
                if (hedges.size() > 0) {
                    FL_LOG("[export warning] multiple hedges are not supported, "
                            "only considering the first hedge except for double hedge 'very'");
                    Hedge* hedge = hedges[0];
                    if (hedge->name() == Not().name()) plusHedge = -1;
                    else if (hedge->name() == Somewhat().name()) plusHedge += 5;
                    else if (hedge->name() == Extremely().name()) plusHedge += 3;
                    else if (hedge->name() == Very().name()) {
                        plusHedge += 2;
                        if (propositions[ixProposition]->hedges.size() > 1
                                and hedges[1]->name() == Very().name())
                            plusHedge += 2;
                    } else plusHedge = std::numeric_limits<scalar>::quiet_NaN();
                }
                break;
            }
            if (fl::Op::isEq(plusHedge, -1)) ss << "-";
            ss << termIndexPlusOne;
            if (not fl::Op::isEq(plusHedge, 0.0))
                ss << "." << fl::Op::str(plusHedge, 0);
            ss << " ";
        }
        return ss.str();
    }

    std::string FisExporter::toFis(const TNorm * tnorm) const {
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

    std::string FisExporter::toFis(const Defuzzifier * defuzzifier) const {
        if (not defuzzifier) return "";
        if (defuzzifier->className() == CenterOfGravity().className()) return "centroid";
        if (defuzzifier->className() == SmallestOfMaximum().className()) return "som";
        if (defuzzifier->className() == LargestOfMaximum().className()) return "lom";
        if (defuzzifier->className() == MeanOfMaximum().className()) return "mom";
        return defuzzifier->className();
    }

    std::string FisExporter::toFis(const Term * term) const {
        std::ostringstream ss;
        if (term->className() == Bell().className()) {
            const Bell* x = dynamic_cast<const Bell*> (term);
            scalar params[] = {x->getWidth(), x->getSlope(), x->getCenter()};
            ss << "'gbellmf',[" << fl::Op::str(3, params, " ") << "]";
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
            scalar params[] = {x->getSigma(), x->getMean()};
            ss << "'gaussmf',[" << fl::Op::str(2, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == GaussianProduct().className()) {
            const GaussianProduct* x = dynamic_cast<const GaussianProduct*> (term);
            scalar params[] = {x->getSigmaA(), x->getMeanA(), x->getSigmaB(), x->getMeanB()};
            ss << "'gauss2mf',[" << fl::Op::str(4, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == PiShape().className()) {
            const PiShape* x = dynamic_cast<const PiShape*> (term);
            scalar params[] = {x->getA(), x->getB(), x->getC(), x->getD()};
            ss << "'pimf',[" << fl::Op::str(4, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == Ramp().className()) {
            const Ramp* x = dynamic_cast<const Ramp*> (term);
            scalar params[] = {x->getStart(), x->getEnd()};
            ss << "'rampmf',[" << fl::Op::str(2, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == Rectangle().className()) {
            const Rectangle* x = dynamic_cast<const Rectangle*> (term);
            scalar params[] = {x->getMinimum(), x->getMaximum()};
            ss << "'rectmf',[" << fl::Op::str(2, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == SShape().className()) {
            const SShape* x = dynamic_cast<const SShape*> (term);
            scalar params[] = {x->getStart(), x->getEnd()};
            ss << "'smf',[" << fl::Op::str(2, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == Sigmoid().className()) {
            const Sigmoid* x = dynamic_cast<const Sigmoid*> (term);
            scalar params[] = {x->getSlope(), x->getInflection()};
            ss << "'sigmf',[" << fl::Op::str(2, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == SigmoidDifference().className()) {
            const SigmoidDifference* x = dynamic_cast<const SigmoidDifference*> (term);
            scalar params[] = {x->getRisingSlope(), x->getLeftInflection(),
                x->getFallingSlope(), x->getRightInflection()};
            ss << "'dsigmf',[" << fl::Op::str(4, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == SigmoidProduct().className()) {
            const SigmoidProduct* x = dynamic_cast<const SigmoidProduct*> (term);
            scalar params[] = {x->getRisingSlope(), x->getLeftInflection(),
                x->getFallingSlope(), x->getRightInflection()};
            ss << "'psigmf',[" << fl::Op::str(4, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == Trapezoid().className()) {
            const Trapezoid* x = dynamic_cast<const Trapezoid*> (term);
            scalar params[] = {x->getA(), x->getB(), x->getC(), x->getD()};
            ss << "'trapmf',[" << fl::Op::str(4, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == Triangle().className()) {
            const Triangle* x = dynamic_cast<const Triangle*> (term);
            scalar params[] = {x->getA(), x->getB(), x->getC()};
            ss << "'trimf',[" << fl::Op::str(3, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == ZShape().className()) {
            const ZShape* x = dynamic_cast<const ZShape*> (term);
            scalar params[] = {x->getStart(), x->getEnd()};
            ss << "'zmf',[" << fl::Op::str(2, params, " ") << "]";
            return ss.str();
        }

        ss << "[export error] term of class <" << term->className() << "> not supported";
        throw fl::Exception(ss.str());
    }
}