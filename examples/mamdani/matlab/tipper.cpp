#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("tipper");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("service");
inputVariable1->setRange(0.000, 10.000);
inputVariable1->addTerm(new Gaussian("poor", 0.000, 1.500));
inputVariable1->addTerm(new Gaussian("good", 5.000, 1.500));
inputVariable1->addTerm(new Gaussian("excellent", 10.000, 1.500));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("food");
inputVariable2->setRange(0.000, 10.000);
inputVariable2->addTerm(new Trapezoid("rancid", 0.000, 0.000, 1.000, 3.000));
inputVariable2->addTerm(new Trapezoid("delicious", 7.000, 9.000, 10.000, 10.000));
engine->addInputVariable(inputVariable2);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("tip");
outputVariable->setRange(0.000, 30.000);
outputVariable->fuzzyOutput()->setAccumulation(new Maximum);
outputVariable->setDefuzzifier(new Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(new Triangle("cheap", 0.000, 5.000, 10.000));
outputVariable->addTerm(new Triangle("average", 10.000, 15.000, 20.000));
outputVariable->addTerm(new Triangle("generous", 20.000, 25.000, 30.000));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setActivation(new Minimum);
ruleBlock->addRule(fl::Rule::parse("if service is poor or food is rancid then tip is cheap", engine));
ruleBlock->addRule(fl::Rule::parse("if service is good then tip is average", engine));
ruleBlock->addRule(fl::Rule::parse("if service is excellent or food is delicious then tip is generous", engine));
engine->addRuleBlock(ruleBlock);


}
