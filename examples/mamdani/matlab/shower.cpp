#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("shower");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("temp");
inputVariable1->setRange(-20.000, 20.000);
inputVariable1->addTerm(new fl::Trapezoid("cold", -30.000, -30.000, -15.000, 0.000));
inputVariable1->addTerm(new fl::Triangle("good", -10.000, 0.000, 10.000));
inputVariable1->addTerm(new fl::Trapezoid("hot", 0.000, 15.000, 30.000, 30.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("flow");
inputVariable2->setRange(-1.000, 1.000);
inputVariable2->addTerm(new fl::Trapezoid("soft", -3.000, -3.000, -0.800, 0.000));
inputVariable2->addTerm(new fl::Triangle("good", -0.400, 0.000, 0.400));
inputVariable2->addTerm(new fl::Trapezoid("hard", 0.000, 0.800, 3.000, 3.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setEnabled(true);
outputVariable1->setName("cold");
outputVariable1->setRange(-1.000, 1.000);
outputVariable1->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable1->setDefuzzifier(new fl::Centroid(200));
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setLockOutputRange(false);
outputVariable1->addTerm(new fl::Triangle("closeFast", -1.000, -0.600, -0.300));
outputVariable1->addTerm(new fl::Triangle("closeSlow", -0.600, -0.300, 0.000));
outputVariable1->addTerm(new fl::Triangle("steady", -0.300, 0.000, 0.300));
outputVariable1->addTerm(new fl::Triangle("openSlow", 0.000, 0.300, 0.600));
outputVariable1->addTerm(new fl::Triangle("openFast", 0.300, 0.600, 1.000));
engine->addOutputVariable(outputVariable1);

fl::OutputVariable* outputVariable2 = new fl::OutputVariable;
outputVariable2->setEnabled(true);
outputVariable2->setName("hot");
outputVariable2->setRange(-1.000, 1.000);
outputVariable2->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable2->setDefuzzifier(new fl::Centroid(200));
outputVariable2->setDefaultValue(fl::nan);
outputVariable2->setLockValidOutput(false);
outputVariable2->setLockOutputRange(false);
outputVariable2->addTerm(new fl::Triangle("closeFast", -1.000, -0.600, -0.300));
outputVariable2->addTerm(new fl::Triangle("closeSlow", -0.600, -0.300, 0.000));
outputVariable2->addTerm(new fl::Triangle("steady", -0.300, 0.000, 0.300));
outputVariable2->addTerm(new fl::Triangle("openSlow", 0.000, 0.300, 0.600));
outputVariable2->addTerm(new fl::Triangle("openFast", 0.300, 0.600, 1.000));
engine->addOutputVariable(outputVariable2);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if temp is cold and flow is soft then cold is openSlow and hot is openFast", engine));
ruleBlock->addRule(fl::Rule::parse("if temp is cold and flow is good then cold is closeSlow and hot is openSlow", engine));
ruleBlock->addRule(fl::Rule::parse("if temp is cold and flow is hard then cold is closeFast and hot is closeSlow", engine));
ruleBlock->addRule(fl::Rule::parse("if temp is good and flow is soft then cold is openSlow and hot is openSlow", engine));
ruleBlock->addRule(fl::Rule::parse("if temp is good and flow is good then cold is steady and hot is steady", engine));
ruleBlock->addRule(fl::Rule::parse("if temp is good and flow is hard then cold is closeSlow and hot is closeSlow", engine));
ruleBlock->addRule(fl::Rule::parse("if temp is hot and flow is soft then cold is openFast and hot is openSlow", engine));
ruleBlock->addRule(fl::Rule::parse("if temp is hot and flow is good then cold is openSlow and hot is closeSlow", engine));
ruleBlock->addRule(fl::Rule::parse("if temp is hot and flow is hard then cold is closeSlow and hot is closeFast", engine));
engine->addRuleBlock(ruleBlock);


}
