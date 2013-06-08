#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("tanksg1");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("level");
inputVariable1->setRange(-1.000, 1.000);

inputVariable1->addTerm(new fl::Gaussian("high", -1.000, 0.300));
inputVariable1->addTerm(new fl::Gaussian("okay", 0.004, 0.300));
inputVariable1->addTerm(new fl::Gaussian("low", 1.000, 0.300));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("rate");
inputVariable2->setRange(-0.100, 0.100);

inputVariable2->addTerm(new fl::Gaussian("negative", -0.100, 0.030));
inputVariable2->addTerm(new fl::Gaussian("none", 0.000, 0.030));
inputVariable2->addTerm(new fl::Gaussian("positive", 0.100, 0.030));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("valve");
outputVariable1->setRange(-1.000, 1.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(new fl::Constant("closefast", -0.900));
outputVariable1->addTerm(new fl::Constant("closeslow", -0.500));
outputVariable1->addTerm(new fl::Constant("nochange", 0.000));
outputVariable1->addTerm(new fl::Constant("openslow", 0.300));
outputVariable1->addTerm(new fl::Constant("openfast", 0.900));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::AlgebraicProduct);
ruleblock1->setSnorm(new fl::AlgebraicSum);
ruleblock1->setActivation(new fl::AlgebraicProduct);

ruleblock1->addRule(fl::FuzzyRule::parse("if level is okay then valve is nochange", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if level is low then valve is openfast", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if level is high then valve is closefast", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if level is okay and rate is positive then valve is closeslow", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if level is okay and rate is negative then valve is openslow", engine));
engine->addRuleBlock(ruleblock1);


}
