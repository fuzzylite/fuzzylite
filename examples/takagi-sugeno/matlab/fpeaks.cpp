#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("peaks");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("in1");
inputVariable1->setRange(-3.000, 3.000);
inputVariable1->addTerm(new fl::Bell("in1mf1", -2.233, 1.578, 2.151));
inputVariable1->addTerm(new fl::Bell("in1mf2", -0.394, 0.753, 1.838));
inputVariable1->addTerm(new fl::Bell("in1mf3", 0.497, 0.689, 1.844));
inputVariable1->addTerm(new fl::Bell("in1mf4", 2.270, 1.528, 2.156));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("in2");
inputVariable2->setRange(-3.000, 3.000);
inputVariable2->addTerm(new fl::Bell("in1mf1", -2.686, 1.267, 2.044));
inputVariable2->addTerm(new fl::Bell("in1mf2", -0.836, 1.266, 1.796));
inputVariable2->addTerm(new fl::Bell("in1mf3", 0.859, 1.314, 1.937));
inputVariable2->addTerm(new fl::Bell("in1mf4", 2.727, 1.214, 2.047));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("out1");
outputVariable->setRange(-10.000, 10.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(fl::Linear::create("out1mf1", engine->inputVariables(), 0.155, -2.228, -8.974));
outputVariable->addTerm(fl::Linear::create("out1mf2", engine->inputVariables(), -0.312, -7.705, -9.055));
outputVariable->addTerm(fl::Linear::create("out1mf3", engine->inputVariables(), -0.454, -4.437, 6.930));
outputVariable->addTerm(fl::Linear::create("out1mf4", engine->inputVariables(), 0.248, -1.122, 5.081));
outputVariable->addTerm(fl::Linear::create("out1mf5", engine->inputVariables(), -6.278, 25.211, 99.148));
outputVariable->addTerm(fl::Linear::create("out1mf6", engine->inputVariables(), 5.531, 105.916, 157.283));
outputVariable->addTerm(fl::Linear::create("out1mf7", engine->inputVariables(), 19.519, 112.333, -127.796));
outputVariable->addTerm(fl::Linear::create("out1mf8", engine->inputVariables(), -5.079, 34.738, -143.414));
outputVariable->addTerm(fl::Linear::create("out1mf9", engine->inputVariables(), -5.889, 27.311, 116.585));
outputVariable->addTerm(fl::Linear::create("out1mf10", engine->inputVariables(), 21.517, 97.266, 93.802));
outputVariable->addTerm(fl::Linear::create("out1mf11", engine->inputVariables(), 9.198, 79.853, -118.482));
outputVariable->addTerm(fl::Linear::create("out1mf12", engine->inputVariables(), -6.571, 23.026, -87.747));
outputVariable->addTerm(fl::Linear::create("out1mf13", engine->inputVariables(), 0.092, -1.126, -4.527));
outputVariable->addTerm(fl::Linear::create("out1mf14", engine->inputVariables(), -0.304, -4.434, -6.561));
outputVariable->addTerm(fl::Linear::create("out1mf15", engine->inputVariables(), -0.166, -6.284, 7.307));
outputVariable->addTerm(fl::Linear::create("out1mf16", engine->inputVariables(), 0.107, -2.028, 8.159));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in1mf1 then out1 is out1mf1", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in1mf2 then out1 is out1mf2", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in1mf3 then out1 is out1mf3", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in1mf4 then out1 is out1mf4", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in1mf1 then out1 is out1mf5", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in1mf2 then out1 is out1mf6", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in1mf3 then out1 is out1mf7", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in1mf4 then out1 is out1mf8", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf3 and in2 is in1mf1 then out1 is out1mf9", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf3 and in2 is in1mf2 then out1 is out1mf10", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf3 and in2 is in1mf3 then out1 is out1mf11", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf3 and in2 is in1mf4 then out1 is out1mf12", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf4 and in2 is in1mf1 then out1 is out1mf13", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf4 and in2 is in1mf2 then out1 is out1mf14", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf4 and in2 is in1mf3 then out1 is out1mf15", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf4 and in2 is in1mf4 then out1 is out1mf16", engine));
engine->addRuleBlock(ruleBlock);


}
