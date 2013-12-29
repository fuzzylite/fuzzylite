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

    FisExporter::FisExporter() {
    }

    FisExporter::~FisExporter() {
    }

    std::string FisExporter::name() const {
        return "FisExporter";
    }

    std::string FisExporter::toString(const Engine* engine) const {
        std::ostringstream fis;
        fis << exportSystem(engine) << "\n";

        fis << exportInputs(engine);

        fis << exportOutputs(engine);

        fis << exportRules(engine);

        return fis.str();
    }

    std::string FisExporter::exportSystem(const Engine* engine) const {
        std::ostringstream fis;
        fis << "[System]\n";
        fis << "Name='" << engine->getName() << "'\n";
        std::string type = "unknown";
        if (engine->numberOfOutputVariables() > 0) {
            int sugeno = 0, mamdani = 0;
            for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
                Defuzzifier* defuzzifier = engine->getOutputVariable(i)->getDefuzzifier();
                if (dynamic_cast<IntegralDefuzzifier*> (defuzzifier)) {
                    ++mamdani;
                } else {
                    ++sugeno;
                }
            }
            if (mamdani > 0 and sugeno == 0) type = "mamdani";
            else if (mamdani == 0 and sugeno > 0) type = "sugeno";
            else type = "unknown";
        }
        fis << "Type='" << type << "'\n";

        //        fis << "Version=" << FL_VERSION << "\n";
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
            std::string tnormClass = (rb->getConjunction() ? rb->getConjunction()->className() : "");
            std::string snormClass = (rb->getDisjunction() ? rb->getDisjunction()->className() : "");
            std::string activationClass = (rb->getActivation() ? rb->getActivation()->className() : "");

            if (not tnorm) tnorm = rb->getConjunction();
            else if (tnorm->className() != tnormClass)
                uniquenessError = "T-Norm";

            if (not snorm) snorm = rb->getDisjunction();
            else if (snorm->className() != snormClass)
                uniquenessError = "S-Norm";

            if (not activation) activation = rb->getActivation();
            else if (activation->className() != activationClass)
                uniquenessError = "activation T-Norm";
            if (not uniquenessError.empty()) break;
        }

        if (not uniquenessError.empty())
            throw fl::Exception("[exporter error] fis files require all ruleblocks "
                "to have the same " + uniquenessError, FL_AT);

        fis << "AndMethod='" << toString(tnorm) << "'\n";
        fis << "OrMethod='" << toString(snorm) << "'\n";
        fis << "ImpMethod='" << toString(activation) << "'\n";

        const SNorm* accumulation = NULL;
        Defuzzifier* defuzzifier = NULL;
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            std::string defuzzClass = outputVariable->getDefuzzifier() ?
                    outputVariable->getDefuzzifier()->className() : "";
            std::string accumClass = outputVariable->fuzzyOutput()->getAccumulation() ?
                    outputVariable->fuzzyOutput()->getAccumulation()->className() : "";

            if (not defuzzifier) defuzzifier = outputVariable->getDefuzzifier();
            else if (defuzzifier->className() != defuzzClass)
                uniquenessError = "defuzzifier";
            if (not accumulation) accumulation = outputVariable->fuzzyOutput()->getAccumulation();
            else if (accumulation->className() != accumClass)
                uniquenessError = "accumulation S-Norm";
            if (not uniquenessError.empty()) break;
        }

        if (not uniquenessError.empty())
            throw fl::Exception("[exporter error] fis files require all output variables "
                "to have the same " + uniquenessError, FL_AT);

        fis << "AggMethod='" << toString(accumulation) << "'\n";
        fis << "DefuzzMethod='" << toString(defuzzifier) << "'\n";
        return fis.str();
    }

    std::string FisExporter::exportInputs(const Engine* engine) const {
        std::ostringstream fis;
        for (int ixVar = 0; ixVar < engine->numberOfInputVariables(); ++ixVar) {
            InputVariable* var = engine->getInputVariable(ixVar);
            fis << "[Input" << (ixVar + 1) << "]\n";
            fis << "Enabled=" << var->isEnabled() << "\n";
            fis << "Name='" << var->getName() << "'\n";
            fis << "Range=[" << fl::Op::join(2, " ", var->getMinimum(), var->getMaximum()) << "]\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int ixTerm = 0; ixTerm < var->numberOfTerms(); ++ixTerm) {
                fis << "MF" << (ixTerm + 1) << "='" << var->getTerm(ixTerm)->getName() << "':"
                        << toString(var->getTerm(ixTerm)) << "\n";
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
            fis << "Enabled=" << var->isEnabled() << "\n";
            fis << "Name='" << var->getName() << "'\n";
            fis << "Range=[" << fl::Op::join(2, " ", var->getMinimum(), var->getMaximum()) << "]\n";
            fis << "Default=" << fl::Op::str(var->getDefaultValue()) << "\n";
            fis << "LockValid=" << var->isLockingValidOutput() << "\n";
            fis << "LockRange=" << var->isLockingOutputRange() << "\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (int ixTerm = 0; ixTerm < var->numberOfTerms(); ++ixTerm) {
                fis << "MF" << (ixTerm + 1) << "='" << var->getTerm(ixTerm)->getName() << "':"
                        << toString(var->getTerm(ixTerm)) << "\n";
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
                fis << exportRule(rb->getRule(ixRule), engine) << "\n";
            }
        }
        return fis.str();
    }

    std::string FisExporter::exportRule(const Rule* rule, const Engine* engine) const {
        if (not rule) return "";
        std::vector<Proposition*> propositions;
        std::vector<Operator*> operators;

        std::queue<Expression*> bfsQueue;
        bfsQueue.push(rule->getAntecedent()->getRoot());
        while (not bfsQueue.empty()) {
            Expression* front = bfsQueue.front();
            bfsQueue.pop();
            if (front->isOperator) {
                Operator* op = dynamic_cast<Operator*> (front);
                bfsQueue.push(op->left);
                bfsQueue.push(op->right);
                operators.push_back(op);
            } else {
                propositions.push_back(dynamic_cast<Proposition*> (front));
            }
        }

        bool equalOperators = true;
        for (std::size_t i = 0; i + 1 < operators.size(); ++i) {
            if (operators.at(i)->name != operators.at(i + 1)->name) {
                equalOperators = false;
                break;
            }
        }
        if (not equalOperators) {
            throw fl::Exception("[exporter error] "
                    "fis files do not support rules with different connectors "
                    "(i.e. ['and', 'or']). All connectors within a rule must be the same", FL_AT);
        }
        std::ostringstream fis;
        std::vector<Variable*> inputVariables, outputVariables;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i)
            inputVariables.push_back(engine->getInputVariable(i));
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i)
            outputVariables.push_back(engine->getOutputVariable(i));

        fis << translate(propositions, inputVariables) << ", ";
        fis << translate(rule->getConsequent()->conclusions(), outputVariables);
        fis << "(" << Op::str(rule->getWeight()) << ") : ";
        if (operators.size() == 0) fis << "1"; //does not matter
        else {
            if (operators.at(0)->name == Rule::FL_AND) fis << "1";
            else if (operators.at(0)->name == Rule::FL_OR) fis << "2";
            else fis << operators.at(0)->name;
        }
        return fis.str();
    }

    std::string FisExporter::translate(const std::vector<Proposition*>& propositions,
            const std::vector<Variable*> variables) const {
        std::ostringstream ss;
        for (std::size_t ixVariable = 0; ixVariable < variables.size(); ++ixVariable) {
            Variable* variable = variables.at(ixVariable);
            int termIndexPlusOne = 0;
            scalar plusHedge = 0;
            int negated = 1;
            for (std::size_t ixProposition = 0; ixProposition < propositions.size(); ++ixProposition) {
                Proposition* proposition = propositions.at(ixProposition);
                if (proposition->variable != variable) continue;

                for (int termIndex = 0; termIndex < variable->numberOfTerms(); ++termIndex) {
                    if (variable->getTerm(termIndex) == proposition->term) {
                        termIndexPlusOne = termIndex + 1;
                        break;
                    }
                }

                std::vector<Hedge*> hedges = proposition->hedges;
                if (hedges.size() > 1) {
                    FL_DBG("[exporter warning] only a few combinations of multiple "
                            "hedges are supported in fis files");
                }
                for (std::size_t ixHedge = 0; ixHedge < hedges.size(); ++ixHedge) {
                    Hedge* hedge = hedges.at(ixHedge);
                    if (hedge->name() == Not().name()) negated *= -1;
                    else if (hedge->name() == Extremely().name()) plusHedge += 0.3;
                    else if (hedge->name() == Very().name()) plusHedge += 0.2;
                    else if (hedge->name() == Somewhat().name()) plusHedge += 0.05;
                    else if (hedge->name() == Seldom().name()) plusHedge += 0.01;
                    else if (hedge->name() == Any().name()) plusHedge += 0.99;
                    else plusHedge = fl::nan; //Unreconized hedge combination (e.g. Any)
                }

                break;
            }
            if (negated < 0) ss << "-";
            if (not fl::Op::isNan(plusHedge)) {
                ss << fl::Op::str(termIndexPlusOne + plusHedge);
            } else {
                ss << termIndexPlusOne << ".?"; // Unreconized hedge combination
            }
            ss << " ";
        }
        return ss.str();
    }

    std::string FisExporter::toString(const TNorm * tnorm) const {
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

    std::string FisExporter::toString(const SNorm * snorm) const {
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

    std::string FisExporter::toString(const Defuzzifier * defuzzifier) const {
        if (not defuzzifier) return "";
        if (defuzzifier->className() == Centroid().className()) return "centroid";
        if (defuzzifier->className() == Bisector().className()) return "bisector";
        if (defuzzifier->className() == LargestOfMaximum().className()) return "lom";
        if (defuzzifier->className() == MeanOfMaximum().className()) return "mom";
        if (defuzzifier->className() == SmallestOfMaximum().className()) return "som";
        if (defuzzifier->className() == WeightedAverage().className()) return "wtaver";
        if (defuzzifier->className() == WeightedSum().className()) return "wtsum";
        return defuzzifier->className();
    }

    std::string FisExporter::toString(const Term * term) const {
        std::ostringstream ss;
        if (term->className() == Bell().className()) {
            const Bell* x = dynamic_cast<const Bell*> (term);
            ss << "'gbellmf',[" << fl::Op::join(3, " ",
                    x->getWidth(), x->getSlope(), x->getCenter()) << "]";
            return ss.str();
        }

        if (term->className() == Constant().className()) {
            const Constant* x = dynamic_cast<const Constant*> (term);
            ss << "'constant',[" << fl::Op::str(x->getValue()) << "]";
            return ss.str();
        }

        if (term->className() == Discrete().className()) {
            ss << "'discretemf',[";
            const Discrete* x = dynamic_cast<const Discrete*> (term);
            for (std::size_t i = 0; i < x->x.size(); ++i) {
                ss << fl::Op::str(x->x.at(i)) << " " << fl::Op::str(x->y.at(i));
                if (i + 1 < x->x.size()) ss << " ";
            }
            ss << "]";
            return ss.str();
        }

        if (term->className() == Function().className()) {
            const Function* x = dynamic_cast<const Function*> (term);
            ss << "'function',[" << x->getFormula() << "]";
            return ss.str();
        }

        if (term->className() == Gaussian().className()) {
            const Gaussian* x = dynamic_cast<const Gaussian*> (term);
            ss << "'gaussmf',[" << fl::Op::join(2, " ",
                    x->getStandardDeviation(), x->getMean()) << "]";
            return ss.str();
        }

        if (term->className() == GaussianProduct().className()) {
            const GaussianProduct* x = dynamic_cast<const GaussianProduct*> (term);
            ss << "'gauss2mf',[" << fl::Op::join(4, " ",
                    x->getStandardDeviationA(), x->getMeanA(),
                    x->getStandardDeviationB(), x->getMeanB()) << "]";
            return ss.str();
        }

        if (term->className() == Linear().className()) {
            const Linear* x = dynamic_cast<const Linear*> (term);
            ss << "'linear',[" << fl::Op::join<scalar>(x->coefficients, " ") << "]";
            return ss.str();
        }


        if (term->className() == PiShape().className()) {
            const PiShape* x = dynamic_cast<const PiShape*> (term);
            ss << "'pimf',[" << fl::Op::join(4, " ",
                    x->getBottomLeft(), x->getTopLeft(),
                    x->getTopRight(), x->getBottomRight()) << "]";
            return ss.str();
        }

        if (term->className() == Ramp().className()) {
            const Ramp* x = dynamic_cast<const Ramp*> (term);
            ss << "'rampmf',[" << fl::Op::join(2, " ",
                    x->getStart(), x->getEnd()) << "]";
            return ss.str();
        }

        if (term->className() == Rectangle().className()) {
            const Rectangle* x = dynamic_cast<const Rectangle*> (term);
            ss << "'rectmf',[" << fl::Op::join(2, " ",
                    x->getStart(), x->getEnd()) << "]";
            return ss.str();
        }

        if (term->className() == SigmoidDifference().className()) {
            const SigmoidDifference* x = dynamic_cast<const SigmoidDifference*> (term);
            ss << "'dsigmf',[" << fl::Op::join(4, " ",
                    x->getRising(), x->getLeft(),
                    x->getFalling(), x->getRight()) << "]";
            return ss.str();
        }

        if (term->className() == Sigmoid().className()) {
            const Sigmoid* x = dynamic_cast<const Sigmoid*> (term);
            ss << "'sigmf',[" << fl::Op::join(2, " ",
                    x->getSlope(), x->getInflection()) << "]";
            return ss.str();
        }

        if (term->className() == SigmoidProduct().className()) {
            const SigmoidProduct* x = dynamic_cast<const SigmoidProduct*> (term);
            ss << "'psigmf',[" << fl::Op::join(4, " ",
                    x->getRising(), x->getLeft(),
                    x->getFalling(), x->getRight()) << "]";
            return ss.str();
        }

        if (term->className() == SShape().className()) {
            const SShape* x = dynamic_cast<const SShape*> (term);
            ss << "'smf',[" << fl::Op::join(2, " ",
                    x->getStart(), x->getEnd()) << "]";
            return ss.str();
        }

        if (term->className() == Trapezoid().className()) {
            const Trapezoid* x = dynamic_cast<const Trapezoid*> (term);
            ss << "'trapmf',[" << fl::Op::join(4, " ",
                    x->getA(), x->getB(), x->getC(), x->getD()) << "]";
            return ss.str();
        }

        if (term->className() == Triangle().className()) {
            const Triangle* x = dynamic_cast<const Triangle*> (term);
            ss << "'trimf',[" << fl::Op::join(3, " ",
                    x->getA(), x->getB(), x->getC()) << "]";
            return ss.str();
        }

        if (term->className() == ZShape().className()) {
            const ZShape* x = dynamic_cast<const ZShape*> (term);
            ss << "'zmf',[" << fl::Op::join(2, " ",
                    x->getStart(), x->getEnd()) << "]";
            return ss.str();
        }

        ss << "[exporter error] term of class <" << term->className() << "> not supported";
        throw fl::Exception(ss.str(), FL_AT);
    }
}