#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("sugeno");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("input");
inputVariable1->setRange(-5.000, 5.000);

inputVariable1->addTerm(new fl::Gaussian("low", -5.000, 4.000));
inputVariable1->addTerm(new fl::Gaussian("high", 5.000, 4.000));
engine->addInputVariable(inputVariable1);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("output");
outputVariable1->setRange(0.000, 1.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("line1", engine->inputVariables(), -1.000, -1.000));
outputVariable1->addTerm(fl::Linear::create("line2", engine->inputVariables(), 1.000, -1.000));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::AlgebraicProduct);
ruleblock1->setSnorm(new fl::AlgebraicSum);
ruleblock1->setActivation(new fl::Minimum);

ruleblock1->addRule(fl::FuzzyRule::parse("if input is low then output is line1", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if input is high then output is line2", engine));
engine->addRuleBlock(ruleblock1);


}
