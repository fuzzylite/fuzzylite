#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("simple-dimmer");

fl::InputVariable* inputVariable = new fl::InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("Ambient");
inputVariable->setRange(0.000, 1.000);
inputVariable->addTerm(new fl::Triangle("DARK", 0.000, 0.250, 0.500));
inputVariable->addTerm(new fl::Triangle("MEDIUM", 0.250, 0.500, 0.750));
inputVariable->addTerm(new fl::Triangle("BRIGHT", 0.500, 0.750, 1.000));
engine->addInputVariable(inputVariable);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("Power");
outputVariable->setRange(0.000, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(new fl::Triangle("LOW", 0.000, 0.250, 0.500));
outputVariable->addTerm(new fl::Triangle("MEDIUM", 0.250, 0.500, 0.750));
outputVariable->addTerm(new fl::Triangle("HIGH", 0.500, 0.750, 1.000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(NULL);
ruleBlock->setDisjunction(NULL);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if Ambient is DARK then Power is HIGH", engine));
ruleBlock->addRule(fl::Rule::parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
ruleBlock->addRule(fl::Rule::parse("if Ambient is BRIGHT then Power is LOW", engine));
engine->addRuleBlock(ruleBlock);


}
