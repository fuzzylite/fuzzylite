#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("linear_tip_calculator");
engine->setDescription("");

InputVariable* FoodQuality = new InputVariable;
FoodQuality->setName("FoodQuality");
FoodQuality->setDescription("");
FoodQuality->setEnabled(true);
FoodQuality->setRange(1.000, 10.000);
FoodQuality->setLockValueInRange(false);
FoodQuality->addTerm(new Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
FoodQuality->addTerm(new Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine->addInputVariable(FoodQuality);

InputVariable* Service = new InputVariable;
Service->setName("Service");
Service->setDescription("");
Service->setEnabled(true);
Service->setRange(1.000, 10.000);
Service->setLockValueInRange(false);
Service->addTerm(new Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
Service->addTerm(new Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine->addInputVariable(Service);

OutputVariable* Tip = new OutputVariable;
Tip->setName("Tip");
Tip->setDescription("");
Tip->setEnabled(true);
Tip->setRange(10.000, 20.000);
Tip->setLockValueInRange(false);
Tip->setAggregation(fl::null);
Tip->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
Tip->setDefaultValue(fl::nan);
Tip->setLockPreviousValue(false);
Tip->addTerm(Linear::create("TenPercent", engine, 0.000, 0.000, 10.000));
Tip->addTerm(Linear::create("FifteenPercent", engine, 0.000, 0.000, 15.000));
Tip->addTerm(Linear::create("TwentyPercent", engine, 0.000, 0.000, 20.000));
engine->addOutputVariable(Tip);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if FoodQuality is Bad and Service is Bad then Tip is TenPercent", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is Bad and Service is Good then Tip is FifteenPercent", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is Good and Service is Bad then Tip is FifteenPercent", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is Good and Service is Good then Tip is TwentyPercent", engine));
engine->addRuleBlock(ruleBlock);


}
