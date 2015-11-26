#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("sugeno");

InputVariable* input = new InputVariable;
input->setEnabled(true);
input->setName("input");
input->setRange(-5.000, 5.000);
input->setLockValueInRange(false);
input->addTerm(new Gaussian("low", -5.000, 4.000));
input->addTerm(new Gaussian("high", 5.000, 4.000));
engine->addInputVariable(input);

OutputVariable* output = new OutputVariable;
output->setEnabled(true);
output->setName("output");
output->setRange(0.000, 1.000);
output->setLockValueInRange(false);
output->fuzzyOutput()->setAccumulation(fl::null);
output->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
output->setDefaultValue(fl::nan);
output->setLockPreviousValue(false);
output->addTerm(Linear::create("line1", engine, -1.000, -1.000));
output->addTerm(Linear::create("line2", engine, 1.000, -1.000));
engine->addOutputVariable(output);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->addRule(fl::Rule::parse("if input is low then output is line1", engine));
ruleBlock->addRule(fl::Rule::parse("if input is high then output is line2", engine));
engine->addRuleBlock(ruleBlock);


}
