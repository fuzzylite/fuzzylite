#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("tipper");
engine->setDescription("");

InputVariable* service = new InputVariable;
service->setName("service");
service->setDescription("");
service->setEnabled(true);
service->setRange(0.000, 10.000);
service->setLockValueInRange(false);
service->addTerm(new Gaussian("poor", 0.000, 1.500));
service->addTerm(new Gaussian("good", 5.000, 1.500));
service->addTerm(new Gaussian("excellent", 10.000, 1.500));
engine->addInputVariable(service);

InputVariable* food = new InputVariable;
food->setName("food");
food->setDescription("");
food->setEnabled(true);
food->setRange(0.000, 10.000);
food->setLockValueInRange(false);
food->addTerm(new Trapezoid("rancid", 0.000, 0.000, 1.000, 3.000));
food->addTerm(new Trapezoid("delicious", 7.000, 9.000, 10.000, 10.000));
engine->addInputVariable(food);

OutputVariable* tip = new OutputVariable;
tip->setName("tip");
tip->setDescription("");
tip->setEnabled(true);
tip->setRange(0.000, 30.000);
tip->setLockValueInRange(false);
tip->setAggregation(new Maximum);
tip->setDefuzzifier(new Centroid(200));
tip->setDefaultValue(fl::nan);
tip->setLockPreviousValue(false);
tip->addTerm(new Triangle("cheap", 0.000, 5.000, 10.000));
tip->addTerm(new Triangle("average", 10.000, 15.000, 20.000));
tip->addTerm(new Triangle("generous", 20.000, 25.000, 30.000));
engine->addOutputVariable(tip);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setImplication(new Minimum);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if service is poor or food is rancid then tip is cheap", engine));
ruleBlock->addRule(Rule::parse("if service is good then tip is average", engine));
ruleBlock->addRule(Rule::parse("if service is excellent or food is delicious then tip is generous", engine));
engine->addRuleBlock(ruleBlock);


}
