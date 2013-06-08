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

inputVariable1->addTerm(new fl::Bell("in1mf1", 1.152, 8.206, 0.874));
inputVariable1->addTerm(new fl::Bell("in1mf2", 15.882, 7.078, 0.444));
inputVariable1->addTerm(new fl::Bell("in1mf3", 30.575, 8.602, 0.818));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("in_n2");
inputVariable2->setRange(1.000, 31.000);

inputVariable2->addTerm(new fl::Bell("in2mf1", 1.426, 8.602, 0.818));
inputVariable2->addTerm(new fl::Bell("in2mf2", 16.118, 7.078, 0.445));
inputVariable2->addTerm(new fl::Bell("in2mf3", 30.847, 8.206, 0.875));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("out1");
outputVariable1->setRange(-0.334, 1.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("out1mf1", engine->inputVariables(), -0.035, 0.002, -0.352));
outputVariable1->addTerm(fl::Linear::create("out1mf2", engine->inputVariables(), 0.044, 0.079, -0.028));
outputVariable1->addTerm(fl::Linear::create("out1mf3", engine->inputVariables(), -0.024, 0.024, -1.599));
outputVariable1->addTerm(fl::Linear::create("out1mf4", engine->inputVariables(), -0.067, 0.384, 0.007));
outputVariable1->addTerm(fl::Linear::create("out1mf5", engine->inputVariables(), 0.351, -0.351, -3.663));
outputVariable1->addTerm(fl::Linear::create("out1mf6", engine->inputVariables(), -0.079, -0.044, 3.909));
outputVariable1->addTerm(fl::Linear::create("out1mf7", engine->inputVariables(), 0.012, -0.012, -0.600));
outputVariable1->addTerm(fl::Linear::create("out1mf8", engine->inputVariables(), -0.384, 0.067, 10.158));
outputVariable1->addTerm(fl::Linear::create("out1mf9", engine->inputVariables(), -0.002, 0.035, -1.402));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::AlgebraicProduct);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::AlgebraicProduct);

ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf1 and in_n2 is in2mf3 then out1 is out1mf3", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf4", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf5", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf2 and in_n2 is in2mf3 then out1 is out1mf6", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf3 and in_n2 is in2mf1 then out1 is out1mf7", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf3 and in_n2 is in2mf2 then out1 is out1mf8", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in_n1 is in1mf3 and in_n2 is in2mf3 then out1 is out1mf9", engine));
engine->addRuleBlock(ruleblock1);


}
