#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("slbb");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("in1");
inputVariable1->setRange(-1.500, 1.500);

inputVariable1->addTerm(new fl::Bell("in1mf1", -1.500, 1.500, 2.000));
inputVariable1->addTerm(new fl::Bell("in1mf2", 1.500, 1.500, 2.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("in2");
inputVariable2->setRange(-1.500, 1.500);

inputVariable2->addTerm(new fl::Bell("in2mf1", -1.500, 1.500, 2.000));
inputVariable2->addTerm(new fl::Bell("in2mf2", 1.500, 1.500, 2.000));
engine->addInputVariable(inputVariable2);

fl::InputVariable* inputVariable3 = new fl::InputVariable;
inputVariable3->setName("in3");
inputVariable3->setRange(-0.200, 0.200);

inputVariable3->addTerm(new fl::Bell("in3mf1", -0.200, 0.200, 2.000));
inputVariable3->addTerm(new fl::Bell("in3mf2", 0.200, 0.200, 2.000));
engine->addInputVariable(inputVariable3);

fl::InputVariable* inputVariable4 = new fl::InputVariable;
inputVariable4->setName("in4");
inputVariable4->setRange(-0.400, 0.400);

inputVariable4->addTerm(new fl::Bell("in4mf1", -0.400, 0.400, 2.000));
inputVariable4->addTerm(new fl::Bell("in4mf2", 0.400, 0.400, 2.000));
engine->addInputVariable(inputVariable4);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("out");
outputVariable1->setRange(-10.000, 10.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("outmf1", engine->inputVariables(), 1.015, 2.234, -12.665, -4.046, 0.026));
outputVariable1->addTerm(fl::Linear::create("outmf2", engine->inputVariables(), 1.161, 1.969, -9.396, -6.165, 0.474));
outputVariable1->addTerm(fl::Linear::create("outmf3", engine->inputVariables(), 1.506, 2.234, -12.990, -1.865, 1.426));
outputVariable1->addTerm(fl::Linear::create("outmf4", engine->inputVariables(), 0.734, 1.969, -9.381, -4.688, -0.880));
outputVariable1->addTerm(fl::Linear::create("outmf5", engine->inputVariables(), 0.734, 2.234, -12.853, -6.110, -1.034));
outputVariable1->addTerm(fl::Linear::create("outmf6", engine->inputVariables(), 1.413, 1.969, -9.485, -6.592, 1.159));
outputVariable1->addTerm(fl::Linear::create("outmf7", engine->inputVariables(), 1.225, 2.234, -12.801, -3.929, 0.366));
outputVariable1->addTerm(fl::Linear::create("outmf8", engine->inputVariables(), 0.985, 1.969, -9.291, -5.115, -0.195));
outputVariable1->addTerm(fl::Linear::create("outmf9", engine->inputVariables(), 0.985, 1.969, -9.292, -5.115, 0.195));
outputVariable1->addTerm(fl::Linear::create("outmf10", engine->inputVariables(), 1.225, 2.234, -12.802, -3.929, -0.366));
outputVariable1->addTerm(fl::Linear::create("outmf11", engine->inputVariables(), 1.413, 1.969, -9.485, -6.592, -1.159));
outputVariable1->addTerm(fl::Linear::create("outmf12", engine->inputVariables(), 0.734, 2.234, -12.853, -6.110, 1.034));
outputVariable1->addTerm(fl::Linear::create("outmf13", engine->inputVariables(), 0.734, 1.969, -9.381, -4.688, 0.880));
outputVariable1->addTerm(fl::Linear::create("outmf14", engine->inputVariables(), 1.506, 2.234, -12.990, -1.865, -1.426));
outputVariable1->addTerm(fl::Linear::create("outmf15", engine->inputVariables(), 1.161, 1.969, -9.396, -6.165, -0.474));
outputVariable1->addTerm(fl::Linear::create("outmf16", engine->inputVariables(), 1.015, 2.234, -12.665, -4.046, -0.026));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::AlgebraicProduct);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::AlgebraicProduct);

ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf1", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf2", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf3", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf4", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf5", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf6", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf7", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf8", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf9", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf10", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf11", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf12", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf13", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf14", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf15", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf16", engine));
engine->addRuleBlock(ruleblock1);


}
