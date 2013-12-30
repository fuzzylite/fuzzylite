#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("Mamdani-Tip-Calculator");

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

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setEnabled(true);
outputVariable1->setName("Tip");
outputVariable1->setRange(0.000, 30.000);
outputVariable1->fuzzyOutput()->setAccumulation(new fl::AlgebraicSum);
outputVariable1->setDefuzzifier(new fl::Centroid(200));
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setLockOutputRange(false);
outputVariable1->addTerm(new fl::Gaussian("AboutTenPercent", 10.000, 2.000));
outputVariable1->addTerm(new fl::Gaussian("AboutFifteenPercent", 15.000, 2.000));
outputVariable1->addTerm(new fl::Gaussian("AboutTwentyPercent", 20.000, 2.000));
engine->addOutputVariable(outputVariable1);

fl::OutputVariable* outputVariable2 = new fl::OutputVariable;
outputVariable2->setEnabled(true);
outputVariable2->setName("CheckPlusTip");
outputVariable2->setRange(1.000, 1.300);
outputVariable2->fuzzyOutput()->setAccumulation(new fl::AlgebraicSum);
outputVariable2->setDefuzzifier(new fl::Centroid(200));
outputVariable2->setDefaultValue(fl::nan);
outputVariable2->setLockValidOutput(false);
outputVariable2->setLockOutputRange(false);
outputVariable2->addTerm(new fl::Gaussian("PlusAboutTenPercent", 1.100, 0.020));
outputVariable2->addTerm(new fl::Gaussian("PlusAboutFifteenPercent", 1.150, 0.020));
outputVariable2->addTerm(new fl::Gaussian("PlusAboutTwentyPercent", 1.200, 0.020));
engine->addOutputVariable(outputVariable2);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Bad and Service is Bad then Tip is AboutTenPercent and CheckPlusTip is PlusAboutTenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Bad and Service is Good then Tip is AboutFifteenPercent and CheckPlusTip is PlusAboutFifteenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is Bad then Tip is AboutFifteenPercent and CheckPlusTip is PlusAboutFifteenPercent", engine));
ruleBlock->addRule(fl::Rule::parse("if FoodQuality is Good and Service is Good then Tip is AboutTwentyPercent and CheckPlusTip is PlusAboutTwentyPercent", engine));
engine->addRuleBlock(ruleBlock);


}
