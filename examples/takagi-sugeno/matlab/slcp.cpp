#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("slcp");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("in1");
inputVariable1->setRange(-0.30000000, 0.30000000);
inputVariable1->addTerm(new fl::Bell("in1mf1", -0.30000000, 0.30000000, 2.00000000));
inputVariable1->addTerm(new fl::Bell("in1mf2", 0.30000000, 0.30000000, 2.00000000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("in2");
inputVariable2->setRange(-1.00000000, 1.00000000);
inputVariable2->addTerm(new fl::Bell("in2mf1", -1.00000000, 1.00000000, 2.00000000));
inputVariable2->addTerm(new fl::Bell("in2mf2", 1.00000000, 1.00000000, 2.00000000));
engine->addInputVariable(inputVariable2);

fl::InputVariable* inputVariable3 = new fl::InputVariable;
inputVariable3->setEnabled(true);
inputVariable3->setName("in3");
inputVariable3->setRange(-3.00000000, 3.00000000);
inputVariable3->addTerm(new fl::Bell("in3mf1", -3.00000000, 3.00000000, 2.00000000));
inputVariable3->addTerm(new fl::Bell("in3mf2", 3.00000000, 3.00000000, 2.00000000));
engine->addInputVariable(inputVariable3);

fl::InputVariable* inputVariable4 = new fl::InputVariable;
inputVariable4->setEnabled(true);
inputVariable4->setName("in4");
inputVariable4->setRange(-3.00000000, 3.00000000);
inputVariable4->addTerm(new fl::Bell("in4mf1", -3.00000000, 3.00000000, 2.00000000));
inputVariable4->addTerm(new fl::Bell("in4mf2", 3.00000000, 3.00000000, 2.00000000));
engine->addInputVariable(inputVariable4);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("out");
outputVariable->setRange(-10.00000000, 10.00000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(fl::Linear::create("outmf1", engine->inputVariables(), 41.37300000, 10.03000000, 3.16200000, 4.28800000, 0.33900000));
outputVariable->addTerm(fl::Linear::create("outmf2", engine->inputVariables(), 40.40900000, 10.05300000, 3.16200000, 4.28800000, 0.20700000));
outputVariable->addTerm(fl::Linear::create("outmf3", engine->inputVariables(), 41.37300000, 10.03000000, 3.16200000, 4.28800000, 0.33900000));
outputVariable->addTerm(fl::Linear::create("outmf4", engine->inputVariables(), 40.40900000, 10.05300000, 3.16200000, 4.28800000, 0.20700000));
outputVariable->addTerm(fl::Linear::create("outmf5", engine->inputVariables(), 38.56100000, 10.17700000, 3.16200000, 4.28800000, -0.04900000));
outputVariable->addTerm(fl::Linear::create("outmf6", engine->inputVariables(), 37.59600000, 10.15400000, 3.16200000, 4.28800000, -0.18100000));
outputVariable->addTerm(fl::Linear::create("outmf7", engine->inputVariables(), 38.56100000, 10.17700000, 3.16200000, 4.28800000, -0.04900000));
outputVariable->addTerm(fl::Linear::create("outmf8", engine->inputVariables(), 37.59600000, 10.15400000, 3.16200000, 4.28800000, -0.18100000));
outputVariable->addTerm(fl::Linear::create("outmf9", engine->inputVariables(), 37.59600000, 10.15400000, 3.16200000, 4.28800000, 0.18100000));
outputVariable->addTerm(fl::Linear::create("outmf10", engine->inputVariables(), 38.56100000, 10.17700000, 3.16200000, 4.28800000, 0.04900000));
outputVariable->addTerm(fl::Linear::create("outmf11", engine->inputVariables(), 37.59600000, 10.15400000, 3.16200000, 4.28800000, 0.18100000));
outputVariable->addTerm(fl::Linear::create("outmf12", engine->inputVariables(), 38.56100000, 10.17700000, 3.16200000, 4.28800000, 0.04900000));
outputVariable->addTerm(fl::Linear::create("outmf13", engine->inputVariables(), 40.40800000, 10.05300000, 3.16200000, 4.28800000, -0.20700000));
outputVariable->addTerm(fl::Linear::create("outmf14", engine->inputVariables(), 41.37300000, 10.03000000, 3.16200000, 4.28800000, -0.33900000));
outputVariable->addTerm(fl::Linear::create("outmf15", engine->inputVariables(), 40.40800000, 10.05300000, 3.16200000, 4.28800000, -0.20700000));
outputVariable->addTerm(fl::Linear::create("outmf16", engine->inputVariables(), 41.37300000, 10.03000000, 3.16200000, 4.28800000, -0.33900000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf1", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf2", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf3", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf4", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf5", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf6", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf7", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf8", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf9", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf10", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf11", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf12", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf13", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf14", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf15", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf16", engine));
engine->addRuleBlock(ruleBlock);


}
