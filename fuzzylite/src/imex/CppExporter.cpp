/* 
 * File:   CppExporter.cpp
 * Author: jcrada
 * 
 * Created on 1 January 2013, 2:41 PM
 */

#include "fl/imex/CppExporter.h"

namespace fl {

    std::string CppExporter::name() const {
        return "CppExporter";
    }

    std::string CppExporter::toString(const Engine* engine) const {
        std::ostringstream cpp;
        cpp << "fl::Engine* engine = new fl::Engine;\n";
        cpp << "engine->setName(\"" << engine->getName() << "\");\n";

        for (int i = 0; i < engine->numberOfHedges(); ++i) {
            cpp << "engine->addHedge(new " << toString(engine->getHedge(i) << ");\n");
        }

        for (int i = 0; i < engine->numberOfInputVariables(); ++i) {
            InputVariable* input = engine->getInputVariable(i);
            cpp << "fl::InputVariable* inputVariable" << (i + 1) << " = new fl::InputVariable;\n";
            cpp << "inputVariable" << (i + 1) << "->setName(\"" << input->getName() << "\");\n";
            cpp << "inputVariable" << (i + 1) << "->setMinimum(" << input->getMinimum() << ");\n";
            cpp << "inputVariable" << (i + 1) << "->setMaximum(" << input->getMaximum() << ");\n";
            for (int t = 0; t < input->numberOfTerms(); ++t) {
                "inputVariable" << (i + 1) << "->addTerm(new fl::" << input->getTerm(i)->toString() << ");\n";
            }
            cpp << "\n";
        }

        for (int i = 0; i < engine->numberOfOutputVariables(); ++i) {
            OutputVariable* output = engine->getOutputVariable(i);
            cpp << "fl::OutputVariable* outputVariable" << (i + 1) << " = new fl::OutputVariable;\n";
            cpp << "outputVariable" << (i + 1) << "->setName(\"" << output->getName() << "\");\n";
            cpp << "outputVariable" << (i + 1) << "->setMinimum(" << output->getMinimum() << ");\n";
            cpp << "outputVariable" << (i + 1) << "->setMaximum(" << output->getMaximum() << ");\n";
            cpp << "outputVariable" << (i + 1) << "->setDefaultValue(" << output->getDefaultValue() << ");\n";
            cpp << "outputVariable" << (i + 1) << "->setLockDefuzzifiedValue(" <<
                    (output->lockDefuzzifiedValue() ? "true" : "false") << ");\n";

            cpp << "outputVariable" << (i + 1) << "->output->setDefuzzifier(new fl::" <<
                    output->getDefuzzifier()->name() << "(" << output->getDefuzzifier()->getDivisions() << ");\n";
            cpp << "outputVariable" << (i + 1) << "->output->setAccumulation(new fl::" <<
                    output->output()->getAccumulation()->className() << ");\n";

            for (int t = 0; t < output->numberOfTerms(); ++t) {
                "outputVariable" << (i + 1) << "->addTerm(new fl::" << output->getTerm(i)->toString() << ");\n";
            }
            cpp << "\n";
        }

        for (int i = 0; i < engine->numberOfRuleBlocks(); ++i) {
            RuleBlock* ruleblock = engine->getRuleBlock(i);
            cpp << "fl::RuleBlock* ruleblock" << (i + 1) << " = new fl::RuleBlock;\n";
            cpp << "ruleblock->setName(\"" << ruleblock->getName() << "\");\n";
            cpp << "ruleblock->setTnorm(new fl::" << ruleblock->getTnorm()->className() << ");\n";
            cpp << "ruleblock->setSnorm(new fl::" << ruleblock->getSnorm()->className() << ");\n";
            cpp << "ruleblock->setActivation(new fl::" << ruleblock->getActivation()->className() << ");\n";

            for (int r = 0; r < ruleblock->numberOfRules(); ++r) {
                cpp << "ruleblock->addRule(fl::MamdaniRule::parse(\"" <<
                        ruleblock->getRule(r)->getUnparsedRule() << "\", engine);\n";
            }
        }

        return cpp.str();
    }

}
