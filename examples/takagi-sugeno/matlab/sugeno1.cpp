#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("sugeno");

InputVariable* inputVariable = new InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("input");
inputVariable->setRange(-5.000, 5.000);
inputVariable->addTerm(new Gaussian("low", -5.000, 4.000));
inputVariable->addTerm(new Gaussian("high", 5.000, 4.000));
engine->addInputVariable(inputVariable);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("output");
outputVariable->setRange(0.000, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(fl::null);
outputVariable->setDefuzzifier(new WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(Linear::create("line1", engine, -1.000, -1.000));
outputVariable->addTerm(Linear::create("line2", engine, 1.000, -1.000));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if input is low then output is line1", engine));
ruleBlock->addRule(fl::Rule::parse("if input is high then output is line2", engine));
engine->addRuleBlock(ruleBlock);


}
