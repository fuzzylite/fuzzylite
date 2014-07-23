#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("simple-dimmer");

InputVariable* inputVariable = new InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("Ambient");
inputVariable->setRange(0.000, 1.000);
inputVariable->addTerm(new Triangle("DARK", 0.000, 0.250, 0.500));
inputVariable->addTerm(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
inputVariable->addTerm(new Triangle("BRIGHT", 0.500, 0.750, 1.000));
engine->addInputVariable(inputVariable);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("Power");
outputVariable->setRange(0.000, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(fl::null);
outputVariable->setDefuzzifier(new WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(new Constant("LOW", 0.250));
outputVariable->addTerm(new Constant("MEDIUM", 0.500));
outputVariable->addTerm(new Constant("HIGH", 0.750));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if Ambient is DARK then Power is HIGH", engine));
ruleBlock->addRule(fl::Rule::parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
ruleBlock->addRule(fl::Rule::parse("if Ambient is BRIGHT then Power is LOW", engine));
engine->addRuleBlock(ruleBlock);


}
