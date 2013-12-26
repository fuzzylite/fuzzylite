#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("simple-dimmer");

fl::InputVariable* inputVariable = new fl::InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("Ambient");
inputVariable->setRange(0.00000000, 1.00000000);
inputVariable->addTerm(new fl::Triangle("DARK", 0.00000000, 0.25000000, 0.50000000));
inputVariable->addTerm(new fl::Triangle("MEDIUM", 0.25000000, 0.50000000, 0.75000000));
inputVariable->addTerm(new fl::Triangle("BRIGHT", 0.50000000, 0.75000000, 1.00000000));
engine->addInputVariable(inputVariable);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("Power");
outputVariable->setRange(0.00000000, 1.00000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(new fl::Triangle("LOW", 0.00000000, 0.25000000, 0.50000000));
outputVariable->addTerm(new fl::Triangle("MEDIUM", 0.25000000, 0.50000000, 0.75000000));
outputVariable->addTerm(new fl::Triangle("HIGH", 0.50000000, 0.75000000, 1.00000000));
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
