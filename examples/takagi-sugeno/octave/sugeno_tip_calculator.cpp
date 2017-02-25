#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("sugeno_tip_calculator");
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

OutputVariable* CheapTip = new OutputVariable;
CheapTip->setName("CheapTip");
CheapTip->setDescription("");
CheapTip->setEnabled(true);
CheapTip->setRange(5.000, 25.000);
CheapTip->setLockValueInRange(false);
CheapTip->setAggregation(fl::null);
CheapTip->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
CheapTip->setDefaultValue(fl::nan);
CheapTip->setLockPreviousValue(false);
CheapTip->addTerm(new Constant("Low", 10.000));
CheapTip->addTerm(new Constant("Medium", 15.000));
CheapTip->addTerm(new Constant("High", 20.000));
engine->addOutputVariable(CheapTip);

OutputVariable* AverageTip = new OutputVariable;
AverageTip->setName("AverageTip");
AverageTip->setDescription("");
AverageTip->setEnabled(true);
AverageTip->setRange(5.000, 25.000);
AverageTip->setLockValueInRange(false);
AverageTip->setAggregation(fl::null);
AverageTip->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
AverageTip->setDefaultValue(fl::nan);
AverageTip->setLockPreviousValue(false);
AverageTip->addTerm(new Constant("Low", 10.000));
AverageTip->addTerm(new Constant("Medium", 15.000));
AverageTip->addTerm(new Constant("High", 20.000));
engine->addOutputVariable(AverageTip);

OutputVariable* GenerousTip = new OutputVariable;
GenerousTip->setName("GenerousTip");
GenerousTip->setDescription("");
GenerousTip->setEnabled(true);
GenerousTip->setRange(5.000, 25.000);
GenerousTip->setLockValueInRange(false);
GenerousTip->setAggregation(fl::null);
GenerousTip->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
GenerousTip->setDefaultValue(fl::nan);
GenerousTip->setLockPreviousValue(false);
GenerousTip->addTerm(new Constant("Low", 10.000));
GenerousTip->addTerm(new Constant("Medium", 15.000));
GenerousTip->addTerm(new Constant("High", 20.000));
engine->addOutputVariable(GenerousTip);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new EinsteinProduct);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if FoodQuality is extremely Bad and Service is extremely Bad then CheapTip is extremely Low and AverageTip is very Low and GenerousTip is Low", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is Good and Service is extremely Bad then CheapTip is Low and AverageTip is Low and GenerousTip is Medium", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is very Good and Service is very Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is High", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is Bad and Service is Bad then CheapTip is Low and AverageTip is Low and GenerousTip is Medium", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is Good and Service is Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is High", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is extremely Good and Service is Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is very High", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is Bad and Service is Good then CheapTip is Low and AverageTip is Medium and GenerousTip is High", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is Good and Service is Good then CheapTip is Medium and AverageTip is Medium and GenerousTip is very High", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is very Bad and Service is very Good then CheapTip is Low and AverageTip is Medium and GenerousTip is High", engine));
ruleBlock->addRule(Rule::parse("if FoodQuality is very very Good and Service is very very Good then CheapTip is High and AverageTip is very High and GenerousTip is extremely High", engine));
engine->addRuleBlock(ruleBlock);


}
