#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("Linear-Tip-Calculator");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("FoodQuality");
inputVariable1->setRange(1.000, 10.000);

inputVariable1->addTerm(new fl::Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
inputVariable1->addTerm(new fl::Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("Service");
inputVariable2->setRange(1.000, 10.000);

inputVariable2->addTerm(new fl::Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
inputVariable2->addTerm(new fl::Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("Tip");
outputVariable1->setRange(10.000, 20.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("TenPercent", engine->inputVariables(), 0.000, 0.000, 10.000));
outputVariable1->addTerm(fl::Linear::create("FifteenPercent", engine->inputVariables(), 0.000, 0.000, 15.000));
outputVariable1->addTerm(fl::Linear::create("TwentyPercent", engine->inputVariables(), 0.000, 0.000, 20.000));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::Minimum);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::Minimum);

ruleblock1->addRule(fl::FuzzyRule::parse("if FoodQuality is Bad and Service is Bad then Tip is TenPercent", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if FoodQuality is Bad and Service is Good then Tip is FifteenPercent", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if FoodQuality is Good and Service is Bad then Tip is FifteenPercent", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if FoodQuality is Good and Service is Good then Tip is TwentyPercent", engine));
engine->addRuleBlock(ruleblock1);


}
