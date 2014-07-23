#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("Sugeno-Tip-Calculator");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("FoodQuality");
inputVariable1->setRange(1.000, 10.000);
inputVariable1->addTerm(new Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
inputVariable1->addTerm(new Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("Service");
inputVariable2->setRange(1.000, 10.000);
inputVariable2->addTerm(new Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
inputVariable2->addTerm(new Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine->addInputVariable(inputVariable2);

OutputVariable* outputVariable1 = new OutputVariable;
outputVariable1->setEnabled(true);
outputVariable1->setName("CheapTip");
outputVariable1->setRange(5.000, 25.000);
outputVariable1->fuzzyOutput()->setAccumulation(fl::null);
outputVariable1->setDefuzzifier(new WeightedAverage);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockPreviousOutputValue(false);
outputVariable1->setLockOutputValueInRange(false);
outputVariable1->addTerm(new Constant("Low", 10.000));
outputVariable1->addTerm(new Constant("Medium", 15.000));
outputVariable1->addTerm(new Constant("High", 20.000));
engine->addOutputVariable(outputVariable1);

OutputVariable* outputVariable2 = new OutputVariable;
outputVariable2->setEnabled(true);
outputVariable2->setName("AverageTip");
outputVariable2->setRange(5.000, 25.000);
outputVariable2->fuzzyOutput()->setAccumulation(fl::null);
outputVariable2->setDefuzzifier(new WeightedAverage);
outputVariable2->setDefaultValue(fl::nan);
outputVariable2->setLockPreviousOutputValue(false);
outputVariable2->setLockOutputValueInRange(false);
outputVariable2->addTerm(new Constant("Low", 10.000));
outputVariable2->addTerm(new Constant("Medium", 15.000));
outputVariable2->addTerm(new Constant("High", 20.000));
engine->addOutputVariable(outputVariable2);

OutputVariable* outputVariable3 = new OutputVariable;
outputVariable3->setEnabled(true);
outputVariable3->setName("GenerousTip");
outputVariable3->setRange(5.000, 25.000);
outputVariable3->fuzzyOutput()->setAccumulation(fl::null);
outputVariable3->setDefuzzifier(new WeightedAverage);
outputVariable3->setDefaultValue(fl::nan);
outputVariable3->setLockPreviousOutputValue(false);
outputVariable3->setLockOutputValueInRange(false);
outputVariable3->addTerm(new Constant("Low", 10.000));
outputVariable3->addTerm(new Constant("Medium", 15.000));
outputVariable3->addTerm(new Constant("High", 20.000));
engine->addOutputVariable(outputVariable3);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new EinsteinProduct);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is extremely Bad and Service is extremely Bad then CheapTip is extremely Low and AverageTip is very Low and GenerousTip is Low", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is extremely Bad then CheapTip is Low and AverageTip is Low and GenerousTip is Medium", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is very Good and Service is very Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is High", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Bad and Service is Bad then CheapTip is Low and AverageTip is Low and GenerousTip is Medium", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is High", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is extremely Good and Service is Bad then CheapTip is Low and AverageTip is Medium and GenerousTip is very High", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Bad and Service is Good then CheapTip is Low and AverageTip is Medium and GenerousTip is High", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is Good then CheapTip is Medium and AverageTip is Medium and GenerousTip is very High", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is very Bad and Service is very Good then CheapTip is Low and AverageTip is Medium and GenerousTip is High", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is very very Good and Service is very very Good then CheapTip is High and AverageTip is very High and GenerousTip is extremely High", engine));
engine->addRuleBlock(ruleBlock);


}
