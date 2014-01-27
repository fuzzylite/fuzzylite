#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("Linear-Tip-Calculator");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("FoodQuality");
inputVariable1->setRange(1.000, 10.000);
inputVariable1->addTerm(new fl::Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
inputVariable1->addTerm(new fl::Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("Service");
inputVariable2->setRange(1.000, 10.000);
inputVariable2->addTerm(new fl::Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
inputVariable2->addTerm(new fl::Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("Tip");
outputVariable->setRange(10.000, 20.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(fl::Linear::create("TenPercent", engine->inputVariables(), 0.000, 0.000, 10.000));
outputVariable->addTerm(fl::Linear::create("FifteenPercent", engine->inputVariables(), 0.000, 0.000, 15.000));
outputVariable->addTerm(fl::Linear::create("TwentyPercent", engine->inputVariables(), 0.000, 0.000, 20.000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Bad and Service is Bad then Tip is TenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Bad and Service is Good then Tip is FifteenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is Bad then Tip is FifteenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is Good then Tip is TwentyPercent", engine));
engine->addRuleBlock(ruleBlock);


}
