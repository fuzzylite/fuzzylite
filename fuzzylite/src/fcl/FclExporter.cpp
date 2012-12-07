/*
 * FclExporter.cpp
 *
 *  Created on: 8/12/2012
 *      Author: jcrada
 */

#include "FclExporter.h"

#include <sstream>

#include "../engine/Engine.h"
#include "../variable/InputVariable.h"
#include "../variable/OutputVariable.h"
#include "../term/Term.h"
#include "../term/Accumulated.h"

#include "../defuzzifier/Defuzzifier.h"
#include "../engine/Operator.h"

#include "../rule/Rule.h"
#include "../rule/RuleBlock.h"

namespace fl {

    std::string FclExporter::toFcl(const Engine* engine) {
        std::stringstream fcl;
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
            if (outputVariable->getDefuzzifier())
                fcl << "METHOD : " << outputVariable->getDefuzzifier()->name() << ";" << std::endl;
            if (outputVariable->getDefuzzifier())
                fcl << "ACCU : " << outputVariable->output()->getAccumulation()->name() << ";" << std::endl;
            fcl << "DEFAULT : " << outputVariable->getDefaultValue() << ";" << std::endl;
            fcl << "END_DEFUZZIFY" << std::endl;
            fcl << std::endl;
        }

        for (int i = 0; i < engine->numberOfRuleblocks(); ++i) {
            RuleBlock* ruleblock = engine->getRuleblock(i);
            fcl << "RULEBLOCK " << ruleblock->getName() << std::endl;
            fcl << "AND : " << ruleblock->getTnorm()->name() << ";" << std::endl;
            fcl << "OR : " << ruleblock->getSnorm()->name() << ";" << std::endl;
            fcl << "ACT : " << ruleblock->getActivation()->name() << ";" << std::endl;
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
