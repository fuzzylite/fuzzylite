/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */
#include "fl/imex/FclExporter.h"

#include "fl/Headers.h"

#include <sstream>

namespace fl {

    FclExporter::FclExporter(const std::string& indent) : Exporter(), _indent(indent) {
    }

    FclExporter::~FclExporter() {
    }

    void FclExporter::setIndent(const std::string& indent) {
        this->_indent = indent;
    }

    std::string FclExporter::getIndent() const {
        return this->_indent;
    }

    std::string FclExporter::name() const {
        return "FclExporter";
    }

    std::string FclExporter::toString(const Engine* engine) const {
        std::ostringstream fcl;
        fcl << "FUNCTION_BLOCK " << engine->getName() << "\n\n";

        fcl << "VAR_INPUT\n";
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            fcl << _indent << Op::validName(engine->getInputVariable(i)->getName()) << ": REAL;\n";
        }
        fcl << "END_VAR\n\n";

        fcl << "VAR_OUTPUT\n";
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            fcl << _indent << Op::validName(engine->getOutputVariable(i)->getName()) << ": REAL;\n";
        }
        fcl << "END_VAR\n\n";

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* inputVariable = engine->getInputVariable(i);
            fcl << toString(inputVariable) << "\n";
        }

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            fcl << toString(outputVariable) << "\n";
        }

        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* ruleblock = engine->getRuleBlock(i);
            fcl << toString(ruleblock) << "\n";
        }

        fcl << "END_FUNCTION_BLOCK\n";
        return fcl.str();
    }

    std::string FclExporter::toString(const InputVariable* inputVariable) const {
        std::ostringstream fcl;
        fcl << "FUZZIFY " << Op::validName(inputVariable->getName()) << "\n";
        if (not inputVariable->isEnabled()) {
            fcl << _indent << "ENABLED : " <<
                    (inputVariable->isEnabled() ? "TRUE" : "FALSE") << ";\n";
        }
        fcl << _indent << "RANGE := (" << fl::Op::join(2, " .. ",
                inputVariable->getMinimum(), inputVariable->getMaximum())
                << ");\n";

        for (int t = 0; t < inputVariable->numberOfTerms(); ++t) {
            Term* term = inputVariable->getTerm(t);
            fcl << _indent << "TERM " << Op::validName(term->getName()) << " := " << toString(term)
                    << ";\n";
        }
        fcl << "END_FUZZIFY\n";
        return fcl.str();
    }

    std::string FclExporter::toString(const OutputVariable* outputVariable) const {
        std::ostringstream fcl;
        fcl << "DEFUZZIFY " << Op::validName(outputVariable->getName()) << "\n";
        if (not outputVariable->isEnabled()) {
            fcl << _indent << "ENABLED : " <<
                    (outputVariable->isEnabled() ? "TRUE" : "FALSE") << ";\n";
        }
        fcl << _indent << "RANGE := (" << fl::Op::join(2, " .. ",
                outputVariable->getMinimum(), outputVariable->getMaximum())
                << ");\n";

        for (int t = 0; t < outputVariable->numberOfTerms(); ++t) {
            Term* term = outputVariable->getTerm(t);
            fcl << _indent << "TERM " << Op::validName(term->getName()) << " := " << toString(term)
                    << ";\n";
        }
        if (outputVariable->getDefuzzifier()) {
            fcl << _indent << "METHOD : " << toString(outputVariable->getDefuzzifier()) << ";\n";
        }
        if (outputVariable->fuzzyOutput()->getAccumulation())
            fcl << _indent << "ACCU : " << toString(outputVariable->fuzzyOutput()->getAccumulation()) << ";\n";

        fcl << _indent << "DEFAULT := " << fl::Op::str(outputVariable->getDefaultValue());
        if (outputVariable->isLockedPreviousOutputValue()) {
            fcl << " | NC";
        }
        fcl << ";\n";

        if (outputVariable->isLockedOutputValueInRange()) {
            fcl << _indent << "LOCK : RANGE;\n";
        }

        fcl << "END_DEFUZZIFY\n";
        return fcl.str();
    }

    std::string FclExporter::toString(const RuleBlock* ruleBlock) const {
        std::ostringstream fcl;
        fcl << "RULEBLOCK " << ruleBlock->getName() << "\n";
        if (not ruleBlock->isEnabled()) {
            fcl << _indent << "ENABLED : " <<
                    (ruleBlock->isEnabled() ? "TRUE" : "FALSE") << ";\n";
        }
        if (ruleBlock->getConjunction())
            fcl << _indent << "AND : " << toString(ruleBlock->getConjunction()) << ";\n";
        if (ruleBlock->getDisjunction())
            fcl << _indent << "OR : " << toString(ruleBlock->getDisjunction()) << ";\n";
        if (ruleBlock->getActivation())
            fcl << _indent << "ACT : " << toString(ruleBlock->getActivation()) << ";\n";

        for (int r = 0; r < ruleBlock->numberOfRules(); ++r) {
            fcl << _indent << "RULE " << (r + 1) << " : " <<
                    ruleBlock->getRule(r)->getText() << "\n";
        }
        fcl << "END_RULEBLOCK\n";
        return fcl.str();
    }
    
    std::string FclExporter::toString(const Norm* norm) const{
        if (not norm) return "NONE";
        
        std::string name = norm->className();
        //TNorms
        if (name == Minimum().className()) return "MIN";
        if (name == AlgebraicProduct().className()) return "PROD";
        if (name == BoundedDifference().className()) return "BDIF";
        if (name == DrasticProduct().className()) return "DPROD";
        if (name == EinsteinProduct().className()) return "EPROD";
        if (name == HamacherProduct().className()) return "HPROD";
        if (name == NilpotentMinimum().className()) return "NMIN";
        
        //SNorms
        if (name == Maximum().className()) return "MAX";
        if (name == AlgebraicSum().className()) return "ASUM";
        if (name == NormalizedSum().className()) return "NSUM";
        if (name == BoundedSum().className()) return "BSUM";
        if (name == DrasticSum().className()) return "DSUM";
        if (name == EinsteinSum().className()) return "ESUM";
        if (name == HamacherSum().className()) return "HSUM";
        if (name == NilpotentMaximum().className()) return "NMAX";
        
        return norm->className();
    }
    
    //TODO: Delete in v6.0
    std::string FclExporter::toString(const TNorm* tnorm) const {
        if (not tnorm) return "NONE";
        std::string name = tnorm->className();
        if (name == Minimum().className()) return "MIN";
        if (name == AlgebraicProduct().className()) return "PROD";
        if (name == BoundedDifference().className()) return "BDIF";
        if (name == DrasticProduct().className()) return "DPROD";
        if (name == EinsteinProduct().className()) return "EPROD";
        if (name == HamacherProduct().className()) return "HPROD";
        if (name == NilpotentMinimum().className()) return "NMIN";
        return tnorm->className();
    }

    //TODO: Delete in v6.0
    std::string FclExporter::toString(const SNorm* snorm) const {
        if (not snorm) return "NONE";
        std::string name = snorm->className();
        if (name == Maximum().className()) return "MAX";
        if (name == AlgebraicSum().className()) return "ASUM";
        if (name == NormalizedSum().className()) return "NSUM";
        if (name == BoundedSum().className()) return "BSUM";
        if (name == DrasticSum().className()) return "DSUM";
        if (name == EinsteinSum().className()) return "ESUM";
        if (name == HamacherSum().className()) return "HSUM";
        if (name == NilpotentMaximum().className()) return "NMAX";
        return snorm->className();
    }

    std::string FclExporter::toString(const Defuzzifier* defuzzifier) const {
        if (not defuzzifier) return "NONE";
        if (defuzzifier->className() == Centroid().className()) return "COG";
        if (defuzzifier->className() == Bisector().className()) return "COA";
        if (defuzzifier->className() == SmallestOfMaximum().className()) return "LM";
        if (defuzzifier->className() == LargestOfMaximum().className()) return "RM";
        if (defuzzifier->className() == MeanOfMaximum().className()) return "MM";
        if (defuzzifier->className() == WeightedAverage().className()) return "COGS";
        if (defuzzifier->className() == WeightedSum().className()) return "COGSS";
        return defuzzifier->className();
    }

    std::string FclExporter::toString(const Term* term) const {
        if (not term) return "";
        if (const Discrete * discrete = dynamic_cast<const Discrete*> (term)) {
            std::ostringstream ss;
            for (std::size_t i = 0; i < discrete->xy().size(); ++i) {
                ss << "(" << fl::Op::str(discrete->xy(i).first) << ", "
                        << fl::Op::str(discrete->xy(i).second) << ")";
                if (i + 1 < discrete->xy().size()) ss << " ";
            }
            return ss.str();
        }

        if (const Constant * constant = dynamic_cast<const Constant*> (term)) {
            return fl::Op::str(constant->getValue());
        }

        std::ostringstream ss;
        ss << term->className() << " " << term->parameters();
        return ss.str();
    }

    FclExporter* FclExporter::clone() const {
        return new FclExporter(*this);
    }


}
