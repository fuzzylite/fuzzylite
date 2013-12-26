#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("tipper");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("service");
inputVariable1->setRange(0.00000000, 10.00000000);
inputVariable1->addTerm(new fl::Gaussian("poor", 0.00000000, 1.50000000));
inputVariable1->addTerm(new fl::Gaussian("good", 5.00000000, 1.50000000));
inputVariable1->addTerm(new fl::Gaussian("excellent", 10.00000000, 1.50000000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("food");
inputVariable2->setRange(0.00000000, 10.00000000);
inputVariable2->addTerm(new fl::Trapezoid("rancid", 0.00000000, 0.00000000, 1.00000000, 3.00000000));
inputVariable2->addTerm(new fl::Trapezoid("delicious", 7.00000000, 9.00000000, 10.00000000, 10.00000000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("tip");
outputVariable->setRange(0.00000000, 30.00000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(new fl::Triangle("cheap", 0.00000000, 5.00000000, 10.00000000));
outputVariable->addTerm(new fl::Triangle("average", 10.00000000, 15.00000000, 20.00000000));
outputVariable->addTerm(new fl::Triangle("generous", 20.00000000, 25.00000000, 30.00000000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if service is poor or food is rancid then tip is cheap", engine));
ruleBlock->addRule(fl::Rule::parse("if service is good then tip is average", engine));
ruleBlock->addRule(fl::Rule::parse("if service is excellent or food is delicious then tip is generous", engine));
engine->addRuleBlock(ruleBlock);


}
