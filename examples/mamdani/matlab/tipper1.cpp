#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("tipper");

fl::InputVariable* inputVariable = new fl::InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("service");
inputVariable->setRange(0.000, 10.000);
inputVariable->addTerm(new fl::Gaussian("poor", 0.000, 1.500));
inputVariable->addTerm(new fl::Gaussian("good", 5.000, 1.500));
inputVariable->addTerm(new fl::Gaussian("excellent", 10.000, 1.500));
engine->addInputVariable(inputVariable);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("tip");
outputVariable->setRange(0.000, 30.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(new fl::Triangle("cheap", 0.000, 5.000, 10.000));
outputVariable->addTerm(new fl::Triangle("average", 10.000, 15.000, 20.000));
outputVariable->addTerm(new fl::Triangle("generous", 20.000, 25.000, 30.000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if service is poor then tip is cheap", engine));
ruleBlock->addRule(fl::Rule::parse("if service is good then tip is average", engine));
ruleBlock->addRule(fl::Rule::parse("if service is excellent then tip is generous", engine));
engine->addRuleBlock(ruleBlock);


}
