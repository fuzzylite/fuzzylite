/*
 * FclExporter.cpp
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#include "fl/fcl/FclExporter.h"

#include "fl/engine/Engine.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/term/Term.h"
#include "fl/term/Accumulated.h"

#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/engine/Operator.h"

#include "fl/rule/Rule.h"
#include "fl/rule/RuleBlock.h"

#include <sstream>

namespace fl {

    std::string FclExporter::toFcl(const Engine* engine) {
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

            fcl << "ACCU : ";
            if (outputVariable->output()->getAccumulation())
                fcl << outputVariable->output()->getAccumulation()->name() << ";";
            fcl << std::endl;

            fcl << "DEFAULT := " << outputVariable->getDefaultValue();
            if (outputVariable->lockDefuzzifiedValue()) {
                fcl << " | NC";
            }
            fcl << ";" << std::endl;

            fcl << "RANGE := (" << outputVariable->getMinimumOutputRange() << " .. "
                    << outputVariable->getMaximumOutputRange() << ");" << std::endl;

            fcl << "END_DEFUZZIFY" << std::endl;
            fcl << std::endl;
        }

        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* ruleblock = engine->getRuleBlock(i);
            fcl << "RULEBLOCK " << ruleblock->getName() << std::endl;
            fcl << "AND : ";
            if (ruleblock->getTnorm())
                fcl << ruleblock->getTnorm()->name();
            fcl << ";" << std::endl;

            fcl << "OR : ";
            if (ruleblock->getSnorm())
                fcl << ruleblock->getSnorm()->name();
            fcl << ";" << std::endl;

            fcl << "ACT : ";
            if (ruleblock->getActivation())
                fcl << ruleblock->getActivation()->name();
            fcl << ";" << std::endl;

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

} /* namespace fl */
