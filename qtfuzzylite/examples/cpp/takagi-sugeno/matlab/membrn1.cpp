#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("anfis");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("in_n1");
inputVariable1->setRange(1.000, 31.000);

inputVariable1->addTerm(new fl::Bell("in1mf1", 2.253, 16.220, 5.050));
inputVariable1->addTerm(new fl::Bell("in1mf2", 31.260, 15.021, 1.843));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("in_n2");
inputVariable2->setRange(1.000, 31.000);

inputVariable2->addTerm(new fl::Bell("in2mf1", 0.740, 15.021, 1.843));
inputVariable2->addTerm(new fl::Bell("in2mf2", 29.747, 16.220, 5.050));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("out1");
outputVariable1->setRange(-0.334, 1.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("out1mf1", engine->inputVariables(), 0.026, 0.071, -0.615));
outputVariable1->addTerm(fl::Linear::create("out1mf2", engine->inputVariables(), -0.036, 0.036, -1.169));
outputVariable1->addTerm(fl::Linear::create("out1mf3", engine->inputVariables(), -0.094, 0.094, 2.231));
outputVariable1->addTerm(fl::Linear::create("out1mf4", engine->inputVariables(), -0.071, -0.026, 2.479));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::AlgebraicProduct);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::AlgebraicProduct);

ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf3", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf4", engine));
engine->addRuleBlock(ruleblock1);


}
