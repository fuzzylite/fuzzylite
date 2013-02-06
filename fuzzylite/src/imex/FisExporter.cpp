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
        std::string uniquenessError;
        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* rb = engine->getRuleBlock(i);
            if (not rb->getTnorm()) continue;
            if (not rb->getSnorm()) continue;
            if (not rb->getActivation()) continue;

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

        if (not uniquenessError.empty())
            throw fl::Exception("[exporter error] fis files require all ruleblocks "
                "to have the same " + uniquenessError, FL_AT);

        fis << "AndMethod='" << toFis(tnorm) << "'\n";
        fis << "OrMethod='" << toFis(snorm) << "'\n";
        fis << "ImpMethod='" << toFis(activation) << "'\n";

        const SNorm* accumulation = NULL;
        Defuzzifier* defuzzifier = NULL;
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            if (not outputVariable->getDefuzzifier()) continue;
            if (not outputVariable->output()->getAccumulation()) continue;

            if (not defuzzifier) defuzzifier = outputVariable->getDefuzzifier();
            else if (defuzzifier->className() != outputVariable->getDefuzzifier()->className())
                uniquenessError = "defuzzifier";
            if (not accumulation) accumulation = outputVariable->output()->getAccumulation();
            else if (accumulation->className() != outputVariable->output()->getAccumulation()->className())
                uniquenessError = "accumulation S-Norm";
            if (not uniquenessError.empty()) break;
        }

        if (not uniquenessError.empty())
            throw fl::Exception("[exporter error] fis files require all ruleblocks "
                "to have the same " + uniquenessError, FL_AT);

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
            fis << "\n";
        }
        return fis.str();
    }

    std::string FisExporter::exportOutputs(const Engine* engine) const {
        std::ostringstream fis;
        for (int ixVar = 0; ixVar < engine->numberOfOutputVariables(); ++ixVar) {
            OutputVariable* var = engine->getOutputVariable(ixVar);
            fis << "[Output" << (ixVar + 1) << "]\n";
            fis << "Name='" << var->getName() << "'\n";
            fis << "Range=[" << var->getMinimum() << " " << var->getMaximum() << "]\n";
            fis << "Default=" << fl::Op::str(var->getDefaultValue()) << "\n";
            fis << "Lock=" << var->lockDefuzzifiedValue() << "\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int ixTerm = 0; ixTerm < var->numberOfTerms(); ++ixTerm) {
                fis << "MF" << (ixTerm + 1) << "='" << var->getTerm(ixTerm)->getName() << "':"
                        << toFis(var->getTerm(ixTerm)) << "\n";
            }
            fis << "\n";
        }
        return fis.str();
    }

    std::string FisExporter::exportRules(const Engine* engine) const {
        std::ostringstream fis;
        fis << "[Rules]\n";
        for (int ixRuleBlock = 0; ixRuleBlock < engine->numberOfRuleBlocks(); ++ixRuleBlock) {
            RuleBlock* rb = engine->getRuleBlock(ixRuleBlock);
            if (engine->numberOfRuleBlocks() > 1) fis << "# RuleBlock " << rb->getName() << "\n";
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
            if (operators.at(i)->name != operators.at(i + 1)->name) {
                equalOperators = false;
                break;
            }
        }
        if (not equalOperators)
            throw fl::Exception("[exporter error] "
                "fis files do not support rules with different connectors "
                "(i.e. ['and', 'or']). All connectors within a rule must be the same", FL_AT);

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
            if (operators.at(0)->name == Rule::FL_AND) fis << "1";
            else if (operators.at(0)->name == Rule::FL_OR) fis << "2";
            else fis << operators.at(0)->name;
        }
        return fis.str();
    }

    std::string FisExporter::translate(const std::vector<MamdaniProposition*>& propositions,
            const std::vector<Variable*> variables) const {
        std::ostringstream ss;
        for (std::size_t ixVariable = 0; ixVariable < variables.size(); ++ixVariable) {
            Variable* variable = variables.at(ixVariable);
            int termIndexPlusOne = 0;
            scalar plusHedge = 0;
            scalar negated = 1;
            for (std::size_t ixProposition = 0; ixProposition < propositions.size(); ++ixProposition) {
                MamdaniProposition* proposition = propositions.at(ixProposition);
                if (proposition->variable != variable) continue;

                for (int termIndex = 0; termIndex < variable->numberOfTerms(); ++termIndex) {
                    if (variable->getTerm(termIndex) == proposition->term) {
                        termIndexPlusOne = termIndex + 1;
                        break;
                    }
                }


                std::vector<Hedge*> hedges = proposition->hedges;
                if (hedges.size() > 0) {
                    FL_LOG("[exporter warning] only a few combinations of multiple "
                            "hedges are supported in fis files");
                }
                for (std::size_t ixHedge = 0; ixHedge < hedges.size(); ++ixHedge) {
                    Hedge* hedge = hedges.at(ixHedge);
                    if (hedge->name() == Not().name()) negated *= -1;
                    else if (hedge->name() == Somewhat().name()) plusHedge += 5;
                    else if (hedge->name() == Extremely().name()) plusHedge += 3;
                    else if (hedge->name() == Very().name()) plusHedge += 2;
                    else plusHedge = std::numeric_limits<scalar>::quiet_NaN();
                }

                break;
            }
            if (negated < 0) ss << "-";
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
        if (name == AlgebraicProduct().className()) return "prod";
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
        if (name == AlgebraicSum().className()) return "sum";
        if (name == BoundedSum().className()) return "bounded_sum";
        if (name == NormalizedSum().className()) return "normalized_sum";
        if (name == DrasticSum().className()) return "drastic_sum";
        if (name == EinsteinSum().className()) return "einstein_sum";
        if (name == HamacherSum().className()) return "hamacher_sum";
        return snorm->className();
    }

    std::string FisExporter::toFis(const Defuzzifier * defuzzifier) const {
        if (not defuzzifier) return "";
        if (defuzzifier->className() == Centroid().className()) return "centroid";
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
                ss << x->x.at(i) << " " << x->y.at(i);
                if (i < x->x.size() - 1) ss << " ";
            }
            ss << "]";
            return ss.str();
        }

        if (term->className() == Gaussian().className()) {
            const Gaussian* x = dynamic_cast<const Gaussian*> (term);
            scalar params[] = {x->getStandardDeviation(), x->getMean()};
            ss << "'gaussmf',[" << fl::Op::str(2, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == GaussianProduct().className()) {
            const GaussianProduct* x = dynamic_cast<const GaussianProduct*> (term);
            scalar params[] = {x->getStandardDeviationA(), x->getMeanA(), x->getStandardDeviationB(), x->getMeanB()};
            ss << "'gauss2mf',[" << fl::Op::str(4, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == PiShape().className()) {
            const PiShape* x = dynamic_cast<const PiShape*> (term);
            scalar params[] = {x->getBottomLeft(), x->getTopLeft(), x->getTopRight(), x->getBottomRight()};
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
            scalar params[] = {x->getRising(), x->getLeft(), x->getFalling(), x->getRight()};
            ss << "'dsigmf',[" << fl::Op::str(4, params, " ") << "]";
            return ss.str();
        }

        if (term->className() == SigmoidProduct().className()) {
            const SigmoidProduct* x = dynamic_cast<const SigmoidProduct*> (term);
            scalar params[] = {x->getRising(), x->getLeft(), x->getFalling(), x->getRight()};
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

        ss << "[exporter error] term of class <" << term->className() << "> not supported";
        throw fl::Exception(ss.str(), FL_AT);
    }
}