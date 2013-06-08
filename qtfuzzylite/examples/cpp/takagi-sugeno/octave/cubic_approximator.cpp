#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("Cubic-Approximator");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("X");
inputVariable1->setRange(-5.000, 5.000);

inputVariable1->addTerm(new fl::Triangle("AboutNegFive", -6.000, -5.000, -4.000));
inputVariable1->addTerm(new fl::Triangle("AboutNegFour", -5.000, -4.000, -3.000));
inputVariable1->addTerm(new fl::Triangle("AboutNegThree", -4.000, -3.000, -2.000));
inputVariable1->addTerm(new fl::Triangle("AboutNegTwo", -3.000, -2.000, -1.000));
inputVariable1->addTerm(new fl::Triangle("AboutNegOne", -2.000, -1.000, 0.000));
inputVariable1->addTerm(new fl::Triangle("AboutZero", -1.000, 0.000, 1.000));
inputVariable1->addTerm(new fl::Triangle("AboutOne", 0.000, 1.000, 2.000));
inputVariable1->addTerm(new fl::Triangle("AboutTwo", 1.000, 2.000, 3.000));
inputVariable1->addTerm(new fl::Triangle("AboutThree", 2.000, 3.000, 4.000));
inputVariable1->addTerm(new fl::Triangle("AboutFour", 3.000, 4.000, 5.000));
inputVariable1->addTerm(new fl::Triangle("AboutFive", 4.000, 5.000, 6.000));
engine->addInputVariable(inputVariable1);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("ApproxXCubed");
outputVariable1->setRange(-5.000, 5.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("TangentatNegFive", engine->inputVariables(), 75.000, 250.000));
outputVariable1->addTerm(fl::Linear::create("TangentatNegFour", engine->inputVariables(), 48.000, 128.000));
outputVariable1->addTerm(fl::Linear::create("TangentatNegThree", engine->inputVariables(), 27.000, 54.000));
outputVariable1->addTerm(fl::Linear::create("TangentatNegTwo", engine->inputVariables(), 12.000, 16.000));
outputVariable1->addTerm(fl::Linear::create("TangentatNegOne", engine->inputVariables(), 3.000, 2.000));
outputVariable1->addTerm(fl::Linear::create("TangentatZero", engine->inputVariables(), 0.000, 0.000));
outputVariable1->addTerm(fl::Linear::create("TangentatOne", engine->inputVariables(), 3.000, -2.000));
outputVariable1->addTerm(fl::Linear::create("TangentatTwo", engine->inputVariables(), 12.000, -16.000));
outputVariable1->addTerm(fl::Linear::create("TangentatThree", engine->inputVariables(), 27.000, -54.000));
outputVariable1->addTerm(fl::Linear::create("TangentatFour", engine->inputVariables(), 48.000, -128.000));
outputVariable1->addTerm(fl::Linear::create("TangentatFive", engine->inputVariables(), 75.000, -250.000));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::Minimum);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::Minimum);

ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutNegFive then ApproxXCubed is TangentatNegFive", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutNegFour then ApproxXCubed is TangentatNegFour", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutNegThree then ApproxXCubed is TangentatNegThree", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutNegTwo then ApproxXCubed is TangentatNegTwo", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutNegOne then ApproxXCubed is TangentatNegOne", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutZero then ApproxXCubed is TangentatZero", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutOne then ApproxXCubed is TangentatOne", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutTwo then ApproxXCubed is TangentatTwo", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutThree then ApproxXCubed is TangentatThree", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutFour then ApproxXCubed is TangentatFour", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if X is AboutFive then ApproxXCubed is TangentatFive", engine));
engine->addRuleBlock(ruleblock1);


}
