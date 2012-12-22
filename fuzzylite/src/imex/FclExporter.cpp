/*
 * FclExporter.cpp
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#include "fl/imex/FclExporter.h"

#include "fl/engine/Engine.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/term/Term.h"
#include "fl/term/Accumulated.h"

#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/operator/Operator.h"
#include "fl/operator/TNorm.h"
#include "fl/operator/SNorm.h"

#include "fl/rule/Rule.h"
#include "fl/rule/RuleBlock.h"

#include <sstream>

namespace fl {

    FclExporter::FclExporter() {
    }

    FclExporter::~FclExporter() {
    }

    std::string FclExporter::toFcl(const TNorm * tnorm) const {
        if (not tnorm) return "";
        std::string name = tnorm->name();
        if (name == Minimum().name()) return "MIN";
        if (name == AlgebraicProduct().name()) return "PROD";
        if (name == BoundedDifference().name()) return "BDIF";
        if (name == DrasticProduct().name()) return "DPROD";
        if (name == EinsteinProduct().name()) return "EPROD";
        if (name == HamacherProduct().name()) return "HPROD";
        std::ostringstream ex;
        ex << "[internal error] T-Norm <" << name << "> not registered in FclExporter";
        throw fl::Exception(ex.str());
    }

    std::string FclExporter::toFcl(const SNorm * snorm) const {
        if (not snorm) return "";
        std::string name = snorm->name();
        if (name == Maximum().name()) return "MAX";
        if (name == AlgebraicSum().name()) return "ASUM";
        if (name == BoundedSum().name()) return "BSUM";
        if (name == DrasticSum().name()) return "DSUM";
        if (name == EinsteinSum().name()) return "ESUM";
        if (name == HamacherSum().name()) return "HSUM";
        std::ostringstream ex;
        ex << "[internal error] S-Norm <" << name << "> not registered in FclExporter";
        throw fl::Exception(ex.str());
    }

    std::string FclExporter::toFcl(const Engine * engine) {
        std::ostringstream fcl;
        fcl << "FUNCTION_BLOCK " << engine->getName() << std::endl;
        fcl << std::endl;

        fcl << "VAR_INPUT" << std::endl;
        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            fcl << engine->getInputVariable(i)->getName() << ": REAL;"
                    << std::endl;
        }
        fcl << "END_VAR" << std::endl;
        fcl << std::endl;

        fcl << "VAR_OUTPUT" << std::endl;
        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            fcl << engine->getOutputVariable(i)->getName() << ": REAL;"
                    << std::endl;
        }
        fcl << "END_VAR" << std::endl;
        fcl << std::endl;

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* inputVariable = engine->getInputVariable(i);
            fcl << "FUZZIFY " << inputVariable->getName() << std::endl;
            fcl << "RANGE := (" << inputVariable->getMinimum() << " .. "
                    << inputVariable->getMaximum() << ");" << std::endl;
            
            for (int t = 0; t < inputVariable->numberOfTerms(); ++t) {
                Term* term = inputVariable->getTerm(t);
                fcl << "TERM " << term->getName() << " := " << term->toString()
                        << ";" << std::endl;
            }
            fcl << "END_FUZZIFY" << std::endl;
            fcl << std::endl;
        }

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* outputVariable = engine->getOutputVariable(i);
            fcl << "DEFUZZIFY " << outputVariable->getName() << std::endl;
            fcl << "RANGE := (" << outputVariable->getMinimum() << " .. "
                    << outputVariable->getMaximum() << ");" << std::endl;
            
            for (int t = 0; t < outputVariable->numberOfTerms(); ++t) {
                Term* term = outputVariable->getTerm(t);
                fcl << "TERM " << term->getName() << " := " << term->toString()
                        << ";" << std::endl;
            }
            fcl << std::endl;

            fcl << "METHOD : ";
            if (outputVariable->getDefuzzifier())
                fcl << outputVariable->getDefuzzifier()->name() << ";";
            fcl << std::endl;

            fcl << "ACCU : " << toFcl(outputVariable->output()->getAccumulation())
                    << ";" << std::endl;

            fcl << "DEFAULT := " << outputVariable->getDefaultValue();
            if (outputVariable->lockDefuzzifiedValue()) {
                fcl << " | NC";
            }
            fcl << ";" << std::endl;

            

            fcl << "END_DEFUZZIFY" << std::endl;
            fcl << std::endl;
        }

        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* ruleblock = engine->getRuleBlock(i);
            fcl << "RULEBLOCK " << ruleblock->getName() << std::endl;

            fcl << "AND : " << toFcl(ruleblock->getTnorm()) << ";" << std::endl;
            fcl << "OR : " << toFcl(ruleblock->getSnorm()) << ";" << std::endl;
            fcl << "ACT : " << toFcl(ruleblock->getActivation()) << ";" << std::endl;

            fcl << std::endl;

            for (int r = 0; r < ruleblock->numberOfRules(); ++r) {
                fcl << "RULE " << (r + 1) << " : " << ruleblock->getRule(r)->toString() << std::endl;
            }
            fcl << "END_RULEBLOCK" << std::endl;
            fcl << std::endl;
        }

        fcl << "END_FUNCTION_BLOCK";
        return fcl.str();
    }

}
