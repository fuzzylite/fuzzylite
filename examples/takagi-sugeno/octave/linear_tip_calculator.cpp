#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("Linear-Tip-Calculator");

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

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("Tip");
outputVariable->setRange(10.000, 20.000);
outputVariable->fuzzyOutput()->setAccumulation(fl::null);
outputVariable->setDefuzzifier(new WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(Linear::create("TenPercent", engine, 0.000, 0.000, 10.000));
outputVariable->addTerm(Linear::create("FifteenPercent", engine, 0.000, 0.000, 15.000));
outputVariable->addTerm(Linear::create("TwentyPercent", engine, 0.000, 0.000, 20.000));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Bad and Service is Bad then Tip is TenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Bad and Service is Good then Tip is FifteenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is Bad then Tip is FifteenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is Good then Tip is TwentyPercent", engine));
engine->addRuleBlock(ruleBlock);


}
