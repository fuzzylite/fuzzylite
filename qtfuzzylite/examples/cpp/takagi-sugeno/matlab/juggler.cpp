#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("juggler");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("xHit");
inputVariable1->setRange(-4.000, 4.000);

inputVariable1->addTerm(new fl::Bell("in1mf1", -4.000, 2.000, 4.000));
inputVariable1->addTerm(new fl::Bell("in1mf2", 0.000, 2.000, 4.000));
inputVariable1->addTerm(new fl::Bell("in1mf3", 4.000, 2.000, 4.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("projectAngle");
inputVariable2->setRange(0.000, 3.142);

inputVariable2->addTerm(new fl::Bell("in2mf1", 0.000, 0.785, 4.000));
inputVariable2->addTerm(new fl::Bell("in2mf2", 1.571, 0.785, 4.000));
inputVariable2->addTerm(new fl::Bell("in2mf3", 3.142, 0.785, 4.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("theta");
outputVariable1->setRange(0.000, 0.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 0.315));
outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 0.315));
outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 0.315));
outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), 0.114, -0.500, 0.785));
outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), 0.114, -0.500, 0.785));
outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), 0.114, -0.500, 0.785));
outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 1.256));
outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 1.256));
outputVariable1->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 1.256));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::AlgebraicProduct);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::AlgebraicProduct);

ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf1 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf1 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf1 and projectAngle is in2mf3 then theta is out1mf", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf2 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf2 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf2 and projectAngle is in2mf3 then theta is out1mf", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf3 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf3 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if xHit is in1mf3 and projectAngle is in2mf3 then theta is out1mf", engine));
engine->addRuleBlock(ruleblock1);


}
