#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("shower");
engine->setDescription("");

InputVariable* temp = new InputVariable;
temp->setName("temp");
temp->setDescription("");
temp->setEnabled(true);
temp->setRange(-20.000, 20.000);
temp->setLockValueInRange(false);
temp->addTerm(new Trapezoid("cold", -30.000, -30.000, -15.000, 0.000));
temp->addTerm(new Triangle("good", -10.000, 0.000, 10.000));
temp->addTerm(new Trapezoid("hot", 0.000, 15.000, 30.000, 30.000));
engine->addInputVariable(temp);

InputVariable* flow = new InputVariable;
flow->setName("flow");
flow->setDescription("");
flow->setEnabled(true);
flow->setRange(-1.000, 1.000);
flow->setLockValueInRange(false);
flow->addTerm(new Trapezoid("soft", -3.000, -3.000, -0.800, 0.000));
flow->addTerm(new Triangle("good", -0.400, 0.000, 0.400));
flow->addTerm(new Trapezoid("hard", 0.000, 0.800, 3.000, 3.000));
engine->addInputVariable(flow);

OutputVariable* cold = new OutputVariable;
cold->setName("cold");
cold->setDescription("");
cold->setEnabled(true);
cold->setRange(-1.000, 1.000);
cold->setLockValueInRange(false);
cold->setAggregation(new Maximum);
cold->setDefuzzifier(new Centroid(200));
cold->setDefaultValue(fl::nan);
cold->setLockPreviousValue(false);
cold->addTerm(new Triangle("closeFast", -1.000, -0.600, -0.300));
cold->addTerm(new Triangle("closeSlow", -0.600, -0.300, 0.000));
cold->addTerm(new Triangle("steady", -0.300, 0.000, 0.300));
cold->addTerm(new Triangle("openSlow", 0.000, 0.300, 0.600));
cold->addTerm(new Triangle("openFast", 0.300, 0.600, 1.000));
engine->addOutputVariable(cold);

OutputVariable* hot = new OutputVariable;
hot->setName("hot");
hot->setDescription("");
hot->setEnabled(true);
hot->setRange(-1.000, 1.000);
hot->setLockValueInRange(false);
hot->setAggregation(new Maximum);
hot->setDefuzzifier(new Centroid(200));
hot->setDefaultValue(fl::nan);
hot->setLockPreviousValue(false);
hot->addTerm(new Triangle("closeFast", -1.000, -0.600, -0.300));
hot->addTerm(new Triangle("closeSlow", -0.600, -0.300, 0.000));
hot->addTerm(new Triangle("steady", -0.300, 0.000, 0.300));
hot->addTerm(new Triangle("openSlow", 0.000, 0.300, 0.600));
hot->addTerm(new Triangle("openFast", 0.300, 0.600, 1.000));
engine->addOutputVariable(hot);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setImplication(new Minimum);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if temp is cold and flow is soft then cold is openSlow and hot is openFast", engine));
ruleBlock->addRule(Rule::parse("if temp is cold and flow is good then cold is closeSlow and hot is openSlow", engine));
ruleBlock->addRule(Rule::parse("if temp is cold and flow is hard then cold is closeFast and hot is closeSlow", engine));
ruleBlock->addRule(Rule::parse("if temp is good and flow is soft then cold is openSlow and hot is openSlow", engine));
ruleBlock->addRule(Rule::parse("if temp is good and flow is good then cold is steady and hot is steady", engine));
ruleBlock->addRule(Rule::parse("if temp is good and flow is hard then cold is closeSlow and hot is closeSlow", engine));
ruleBlock->addRule(Rule::parse("if temp is hot and flow is soft then cold is openFast and hot is openSlow", engine));
ruleBlock->addRule(Rule::parse("if temp is hot and flow is good then cold is openSlow and hot is closeSlow", engine));
ruleBlock->addRule(Rule::parse("if temp is hot and flow is hard then cold is closeSlow and hot is closeFast", engine));
engine->addRuleBlock(ruleBlock);


}
