#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("sugeno tipper");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("service");
inputVariable1->setRange(0.000, 10.000);

inputVariable1->addTerm(new fl::Gaussian("poor", 0.000, 1.500));
inputVariable1->addTerm(new fl::Gaussian("average", 5.000, 1.500));
inputVariable1->addTerm(new fl::Gaussian("good", 10.000, 1.500));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("food");
inputVariable2->setRange(0.000, 10.000);

inputVariable2->addTerm(new fl::Trapezoid("rancid", -5.000, 0.000, 1.000, 3.000));
inputVariable2->addTerm(new fl::Trapezoid("delicious", 7.000, 9.000, 10.000, 15.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("tip");
outputVariable1->setRange(-30.000, 30.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("cheap", engine->inputVariables(), 0.000, 0.000, 5.000));
outputVariable1->addTerm(fl::Linear::create("average", engine->inputVariables(), 0.000, 0.000, 15.000));
outputVariable1->addTerm(fl::Linear::create("generous", engine->inputVariables(), 0.000, 0.000, 25.000));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::Minimum);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::Minimum);

ruleblock1->addRule(fl::FuzzyRule::parse("if service is poor or food is rancid then tip is cheap", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if service is average then tip is average", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if service is good or food is delicious then tip is generous", engine));
engine->addRuleBlock(ruleblock1);


}
