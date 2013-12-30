#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("sugeno");

fl::InputVariable* inputVariable = new fl::InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("input");
inputVariable->setRange(-5.000, 5.000);
inputVariable->addTerm(new fl::Gaussian("low", -5.000, 4.000));
inputVariable->addTerm(new fl::Gaussian("high", 5.000, 4.000));
engine->addInputVariable(inputVariable);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("output");
outputVariable->setRange(0.000, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(fl::Linear::create("line1", engine->inputVariables(), -1.000, -1.000));
outputVariable->addTerm(fl::Linear::create("line2", engine->inputVariables(), 1.000, -1.000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::AlgebraicSum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if input is low then output is line1", engine));
ruleBlock->addRule(fl::Rule::parse("if input is high then output is line2", engine));
engine->addRuleBlock(ruleBlock);


}
