#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("slcp");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("in1");
inputVariable1->setRange(-0.300, 0.300);

inputVariable1->addTerm(new fl::Bell("in1mf1", -0.300, 0.300, 2.000));
inputVariable1->addTerm(new fl::Bell("in1mf2", 0.300, 0.300, 2.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("in2");
inputVariable2->setRange(-1.000, 1.000);

inputVariable2->addTerm(new fl::Bell("in2mf1", -1.000, 1.000, 2.000));
inputVariable2->addTerm(new fl::Bell("in2mf2", 1.000, 1.000, 2.000));
engine->addInputVariable(inputVariable2);

fl::InputVariable* inputVariable3 = new fl::InputVariable;
inputVariable3->setName("in3");
inputVariable3->setRange(-3.000, 3.000);

inputVariable3->addTerm(new fl::Bell("in3mf1", -3.000, 3.000, 2.000));
inputVariable3->addTerm(new fl::Bell("in3mf2", 3.000, 3.000, 2.000));
engine->addInputVariable(inputVariable3);

fl::InputVariable* inputVariable4 = new fl::InputVariable;
inputVariable4->setName("in4");
inputVariable4->setRange(-3.000, 3.000);

inputVariable4->addTerm(new fl::Bell("in4mf1", -3.000, 3.000, 2.000));
inputVariable4->addTerm(new fl::Bell("in4mf2", 3.000, 3.000, 2.000));
engine->addInputVariable(inputVariable4);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("out");
outputVariable1->setRange(-10.000, 10.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("outmf1", engine->inputVariables(), 41.373, 10.030, 3.162, 4.288, 0.339));
outputVariable1->addTerm(fl::Linear::create("outmf2", engine->inputVariables(), 40.409, 10.053, 3.162, 4.288, 0.207));
outputVariable1->addTerm(fl::Linear::create("outmf3", engine->inputVariables(), 41.373, 10.030, 3.162, 4.288, 0.339));
outputVariable1->addTerm(fl::Linear::create("outmf4", engine->inputVariables(), 40.409, 10.053, 3.162, 4.288, 0.207));
outputVariable1->addTerm(fl::Linear::create("outmf5", engine->inputVariables(), 38.561, 10.177, 3.162, 4.288, -0.049));
outputVariable1->addTerm(fl::Linear::create("outmf6", engine->inputVariables(), 37.596, 10.154, 3.162, 4.288, -0.181));
outputVariable1->addTerm(fl::Linear::create("outmf7", engine->inputVariables(), 38.561, 10.177, 3.162, 4.288, -0.049));
outputVariable1->addTerm(fl::Linear::create("outmf8", engine->inputVariables(), 37.596, 10.154, 3.162, 4.288, -0.181));
outputVariable1->addTerm(fl::Linear::create("outmf9", engine->inputVariables(), 37.596, 10.154, 3.162, 4.288, 0.181));
outputVariable1->addTerm(fl::Linear::create("outmf10", engine->inputVariables(), 38.561, 10.177, 3.162, 4.288, 0.049));
outputVariable1->addTerm(fl::Linear::create("outmf11", engine->inputVariables(), 37.596, 10.154, 3.162, 4.288, 0.181));
outputVariable1->addTerm(fl::Linear::create("outmf12", engine->inputVariables(), 38.561, 10.177, 3.162, 4.288, 0.049));
outputVariable1->addTerm(fl::Linear::create("outmf13", engine->inputVariables(), 40.408, 10.053, 3.162, 4.288, -0.207));
outputVariable1->addTerm(fl::Linear::create("outmf14", engine->inputVariables(), 41.373, 10.030, 3.162, 4.288, -0.339));
outputVariable1->addTerm(fl::Linear::create("outmf15", engine->inputVariables(), 40.408, 10.053, 3.162, 4.288, -0.207));
outputVariable1->addTerm(fl::Linear::create("outmf16", engine->inputVariables(), 41.373, 10.030, 3.162, 4.288, -0.339));
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
