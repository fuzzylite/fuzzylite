/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/imex/FisExporter.h"

#include "fl/Headers.h"

#include <queue>

namespace fl {

    FisExporter::FisExporter() : Exporter() {
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
        std::string type;
        if (engine->type() == Engine::Mamdani or engine->type() == Engine::Larsen) {
            type = "mamdani";
        } else if (engine->type() == Engine::TakagiSugeno) {
            type = "sugeno";
        } else if (engine->type() == Engine::Tsukamoto) {
            type = "tsukamoto";
        } else if (engine->type() == Engine::InverseTsukamoto) {
            type = "inverse tsukamoto";
        } else if (engine->type() == Engine::Hybrid) {
            type = "hybrid";
        } else {
            type = "unknown";
        }
        fis << "Type='" << type << "'\n";
        //        fis << "Version=" << FL_VERSION << "\n";
        fis << "NumInputs=" << engine->numberOfInputVariables() << "\n";
        fis << "NumOutputs=" << engine->numberOfOutputVariables() << "\n";

        std::size_t numberOfRules = 0;
        const TNorm* conjunction = fl::null;
        const SNorm* disjunction = fl::null;
        const TNorm* implication = fl::null;
        for (std::size_t i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* rb = engine->getRuleBlock(i);
            numberOfRules += rb->numberOfRules();
            if (not conjunction) conjunction = rb->getConjunction();
            if (not disjunction) disjunction = rb->getDisjunction();
            if (not implication) implication = rb->getImplication();
        }
        fis << "NumRules=" << numberOfRules << "\n";
        fis << "AndMethod='" << toString(conjunction) << "'\n";
        fis << "OrMethod='" << toString(disjunction) << "'\n";
        fis << "ImpMethod='" << toString(implication) << "'\n";

        const SNorm* aggregation = fl::null;
        Defuzzifier* defuzzifier = fl::null;
        for (std::size_t i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            if (not aggregation) aggregation = outputVariable->fuzzyOutput()->getAggregation();
            if (not defuzzifier) defuzzifier = outputVariable->getDefuzzifier();
        }

        fis << "AggMethod='" << toString(aggregation) << "'\n";
        fis << "DefuzzMethod='" << toString(defuzzifier) << "'\n";
        return fis.str();
    }

    std::string FisExporter::exportInputs(const Engine* engine) const {
        std::ostringstream fis;
        for (std::size_t ixVar = 0; ixVar < engine->numberOfInputVariables(); ++ixVar) {
            InputVariable* var = engine->getInputVariable(ixVar);
            fis << "[Input" << (ixVar + 1) << "]\n";
            fis << "Name='" << Op::validName(var->getName()) << "'\n";
            fis << "Range=[" << fl::Op::join(2, " ", var->getMinimum(), var->getMaximum()) << "]\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (std::size_t ixTerm = 0; ixTerm < var->numberOfTerms(); ++ixTerm) {
                fis << "MF" << (ixTerm + 1) << "='" << Op::validName(var->getTerm(ixTerm)->getName()) << "':"
                        << toString(var->getTerm(ixTerm)) << "\n";
            }
            fis << "\n";
        }
        return fis.str();
    }

    std::string FisExporter::exportOutputs(const Engine* engine) const {
        std::ostringstream fis;
        for (std::size_t ixVar = 0; ixVar < engine->numberOfOutputVariables(); ++ixVar) {
            OutputVariable* var = engine->getOutputVariable(ixVar);
            fis << "[Output" << (ixVar + 1) << "]\n";
            fis << "Name='" << Op::validName(var->getName()) << "'\n";
            fis << "Range=[" << fl::Op::join(2, " ", var->getMinimum(), var->getMaximum()) << "]\n";
            fis << "NumMFs=" << var->numberOfTerms() << "\n";
            for (std::size_t ixTerm = 0; ixTerm < var->numberOfTerms(); ++ixTerm) {
                fis << "MF" << (ixTerm + 1) << "='" << Op::validName(var->getTerm(ixTerm)->getName()) << "':"
                        << toString(var->getTerm(ixTerm)) << "\n";
            }
            fis << "\n";
        }
        return fis.str();
    }

    std::string FisExporter::exportRules(const Engine* engine) const {
        std::ostringstream fis;
        fis << "[Rules]\n";
        for (std::size_t ixRuleBlock = 0; ixRuleBlock < engine->numberOfRuleBlocks(); ++ixRuleBlock) {
            RuleBlock* rb = engine->getRuleBlock(ixRuleBlock);
            if (engine->numberOfRuleBlocks() > 1) fis << "# RuleBlock " << rb->getName() << "\n";
            for (std::size_t ixRule = 0; ixRule < rb->numberOfRules(); ++ixRule) {
                Rule* rule = rb->getRule(ixRule);
                if (rule->isLoaded()) {
                    fis << exportRule(rule, engine) << "\n";
                }
            }
        }
        return fis.str();
    }

    std::string FisExporter::exportRule(const Rule* rule, const Engine* engine) const {
        if (not rule) return "";
        std::vector<Proposition*> propositions;
        std::vector<Operator*> operators;

        std::queue<Expression*> bfsQueue;
        bfsQueue.push(rule->getAntecedent()->getExpression());
        while (not bfsQueue.empty()) {
            Expression* front = bfsQueue.front();
            bfsQueue.pop();
            Operator* op = dynamic_cast<Operator*> (front);
            if (op) {
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
        for (std::size_t i = 0; i < engine->numberOfInputVariables(); ++i)
            inputVariables.push_back(engine->getInputVariable(i));
        for (std::size_t i = 0; i < engine->numberOfOutputVariables(); ++i)
            outputVariables.push_back(engine->getOutputVariable(i));

        fis << translate(propositions, inputVariables) << ", ";
        fis << translate(rule->getConsequent()->conclusions(), outputVariables);
        fis << "(" << Op::str(rule->getWeight()) << ") : ";
        if (operators.size() == 0) fis << "1"; //does not matter
        else {
            if (operators.at(0)->name == Rule::andKeyword()) fis << "1";
            else if (operators.at(0)->name == Rule::orKeyword()) fis << "2";
            else fis << operators.at(0)->name;
        }
        return fis.str();
    }

    std::string FisExporter::translate(const std::vector<Proposition*>& propositions,
            const std::vector<Variable*> variables) const {
        std::ostringstream ss;
        for (std::size_t ixVariable = 0; ixVariable < variables.size(); ++ixVariable) {
            Variable* variable = variables.at(ixVariable);
            std::size_t termIndexPlusOne = 0;
            scalar plusHedge = 0;
            int negated = 1;
            for (std::size_t ixProposition = 0; ixProposition < propositions.size(); ++ixProposition) {
                Proposition* proposition = propositions.at(ixProposition);
                if (proposition->variable != variable) continue;

                for (std::size_t termIndex = 0; termIndex < variable->numberOfTerms(); ++termIndex) {
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
            if (not fl::Op::isNaN(plusHedge)) {
                ss << fl::Op::str(termIndexPlusOne + plusHedge);
            } else {
                ss << termIndexPlusOne << ".?"; // Unreconized hedge combination
            }
            ss << " ";
        }
        return ss.str();
    }

    std::string FisExporter::toString(const Norm * norm) const {
        if (not norm) return "";
        //TNorm
        if (norm->className() == Minimum().className()) return "min";
        if (norm->className() == AlgebraicProduct().className()) return "prod";
        if (norm->className() == BoundedDifference().className()) return "bounded_difference";
        if (norm->className() == DrasticProduct().className()) return "drastic_product";
        if (norm->className() == EinsteinProduct().className()) return "einstein_product";
        if (norm->className() == HamacherProduct().className()) return "hamacher_product";
        if (norm->className() == NilpotentMinimum().className()) return "nilpotent_minimum";
        //SNorm
        if (norm->className() == Maximum().className()) return "max";
        if (norm->className() == AlgebraicSum().className()) return "probor";
        if (norm->className() == BoundedSum().className()) return "bounded_sum";
        if (norm->className() == NormalizedSum().className()) return "normalized_sum";
        if (norm->className() == DrasticSum().className()) return "drastic_sum";
        if (norm->className() == EinsteinSum().className()) return "einstein_sum";
        if (norm->className() == HamacherSum().className()) return "hamacher_sum";
        if (norm->className() == NilpotentMaximum().className()) return "nilpotent_maximum";
        if (norm->className() == UnboundedSum().className()) return "sum";

        return norm->className();
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
        if (const Bell * x = dynamic_cast<const Bell*> (term)) {
            ss << "'gbellmf',[" << fl::Op::join(3, " ",
                    x->getWidth(), x->getSlope(), x->getCenter()) << "]";
            return ss.str();
        }

        if (const Binary * x = dynamic_cast<const Binary*> (term)) {
            ss << "'binarymf,[" << fl::Op::join(2, " ", 
                    x->getStart(), x->getDirection()) << "]";
            return ss.str();
        }

        if (const Concave * x = dynamic_cast<const Concave*> (term)) {
            ss << "'concavemf',[" << fl::Op::join(2, " ",
                    x->getInflection(), x->getEnd()) << "]";
            return ss.str();
        }

        if (const Constant * x = dynamic_cast<const Constant*> (term)) {
            ss << "'constant',[" << fl::Op::str(x->getValue()) << "]";
            return ss.str();
        }

        if (const Cosine * x = dynamic_cast<const Cosine*> (term)) {
            ss << "'cosinemf',[" << fl::Op::join(2, " ",
                    x->getCenter(), x->getWidth()) << "]";
            return ss.str();
        }

        if (const Discrete * x = dynamic_cast<const Discrete*> (term)) {
            ss << "'discretemf',[" << fl::Op::join(Discrete::toVector(x->xy()), " ") << "]";
            return ss.str();
        }

        if (const Function * x = dynamic_cast<const Function*> (term)) {
            ss << "'function',[" << x->getFormula() << "]";
            return ss.str();
        }

        if (const Gaussian * x = dynamic_cast<const Gaussian*> (term)) {
            ss << "'gaussmf',[" << fl::Op::join(2, " ",
                    x->getStandardDeviation(), x->getMean()) << "]";
            return ss.str();
        }

        if (const GaussianProduct * x = dynamic_cast<const GaussianProduct*> (term)) {
            ss << "'gauss2mf',[" << fl::Op::join(4, " ",
                    x->getStandardDeviationA(), x->getMeanA(),
                    x->getStandardDeviationB(), x->getMeanB()) << "]";
            return ss.str();
        }

        if (const Linear * x = dynamic_cast<const Linear*> (term)) {
            ss << "'linear',[" << fl::Op::join<scalar>(x->coefficients(), " ") << "]";
            return ss.str();
        }


        if (const PiShape * x = dynamic_cast<const PiShape*> (term)) {
            ss << "'pimf',[" << fl::Op::join(4, " ",
                    x->getBottomLeft(), x->getTopLeft(),
                    x->getTopRight(), x->getBottomRight()) << "]";
            return ss.str();
        }

        if (const Ramp * x = dynamic_cast<const Ramp*> (term)) {
            ss << "'rampmf',[" << fl::Op::join(2, " ",
                    x->getStart(), x->getEnd()) << "]";
            return ss.str();
        }

        if (const Rectangle * x = dynamic_cast<const Rectangle*> (term)) {
            ss << "'rectmf',[" << fl::Op::join(2, " ",
                    x->getStart(), x->getEnd()) << "]";
            return ss.str();
        }

        if (const SigmoidDifference * x = dynamic_cast<const SigmoidDifference*> (term)) {
            ss << "'dsigmf',[" << fl::Op::join(4, " ",
                    x->getRising(), x->getLeft(),
                    x->getFalling(), x->getRight()) << "]";
            return ss.str();
        }

        if (const Sigmoid * x = dynamic_cast<const Sigmoid*> (term)) {
            ss << "'sigmf',[" << fl::Op::join(2, " ",
                    x->getSlope(), x->getInflection()) << "]";
            return ss.str();
        }

        if (const SigmoidProduct * x = dynamic_cast<const SigmoidProduct*> (term)) {
            ss << "'psigmf',[" << fl::Op::join(4, " ",
                    x->getRising(), x->getLeft(),
                    x->getFalling(), x->getRight()) << "]";
            return ss.str();
        }

        if (const SShape * x = dynamic_cast<const SShape*> (term)) {
            ss << "'smf',[" << fl::Op::join(2, " ",
                    x->getStart(), x->getEnd()) << "]";
            return ss.str();
        }

        if (const Spike * x = dynamic_cast<const Spike*> (term)) {
            ss << "'spikemf',[" << fl::Op::join(2, " ",
                    x->getCenter(), x->getWidth()) << "]";
            return ss.str();
        }

        if (const Trapezoid * x = dynamic_cast<const Trapezoid*> (term)) {
            ss << "'trapmf',[" << fl::Op::join(4, " ",
                    x->getVertexA(), x->getVertexB(), x->getVertexC(), x->getVertexD()) << "]";
            return ss.str();
        }

        if (const Triangle * x = dynamic_cast<const Triangle*> (term)) {
            ss << "'trimf',[" << fl::Op::join(3, " ",
                    x->getVertexA(), x->getVertexB(), x->getVertexC()) << "]";
            return ss.str();
        }

        if (const ZShape * x = dynamic_cast<const ZShape*> (term)) {
            ss << "'zmf',[" << fl::Op::join(2, " ",
                    x->getStart(), x->getEnd()) << "]";
            return ss.str();
        }

        ss << "[exporter error] term of class <" << term->className() << "> not supported";
        throw fl::Exception(ss.str(), FL_AT);
    }

    FisExporter* FisExporter::clone() const {
        return new FisExporter(*this);
    }

}
