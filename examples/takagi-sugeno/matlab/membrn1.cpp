#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("anfis");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("in_n1");
inputVariable1->setRange(1.00000000, 31.00000000);
inputVariable1->addTerm(new fl::Bell("in1mf1", 2.25300000, 16.22000000, 5.05000000));
inputVariable1->addTerm(new fl::Bell("in1mf2", 31.26000000, 15.02100000, 1.84300000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("in_n2");
inputVariable2->setRange(1.00000000, 31.00000000);
inputVariable2->addTerm(new fl::Bell("in2mf1", 0.74000000, 15.02100000, 1.84300000));
inputVariable2->addTerm(new fl::Bell("in2mf2", 29.74700000, 16.22000000, 5.05000000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("out1");
outputVariable->setRange(-0.33400000, 1.00000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(fl::Linear::create("out1mf1", engine->inputVariables(), 0.02600000, 0.07100000, -0.61500000));
outputVariable->addTerm(fl::Linear::create("out1mf2", engine->inputVariables(), -0.03600000, 0.03600000, -1.16900000));
outputVariable->addTerm(fl::Linear::create("out1mf3", engine->inputVariables(), -0.09400000, 0.09400000, 2.23100000));
outputVariable->addTerm(fl::Linear::create("out1mf4", engine->inputVariables(), -0.07100000, -0.02600000, 2.47900000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf3", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf4", engine));
engine->addRuleBlock(ruleBlock);


}
