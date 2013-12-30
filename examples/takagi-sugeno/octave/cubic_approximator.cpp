#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("Cubic-Approximator");

fl::InputVariable* inputVariable = new fl::InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("X");
inputVariable->setRange(-5.000, 5.000);
inputVariable->addTerm(new fl::Triangle("AboutNegFive", -6.000, -5.000, -4.000));
inputVariable->addTerm(new fl::Triangle("AboutNegFour", -5.000, -4.000, -3.000));
inputVariable->addTerm(new fl::Triangle("AboutNegThree", -4.000, -3.000, -2.000));
inputVariable->addTerm(new fl::Triangle("AboutNegTwo", -3.000, -2.000, -1.000));
inputVariable->addTerm(new fl::Triangle("AboutNegOne", -2.000, -1.000, 0.000));
inputVariable->addTerm(new fl::Triangle("AboutZero", -1.000, 0.000, 1.000));
inputVariable->addTerm(new fl::Triangle("AboutOne", 0.000, 1.000, 2.000));
inputVariable->addTerm(new fl::Triangle("AboutTwo", 1.000, 2.000, 3.000));
inputVariable->addTerm(new fl::Triangle("AboutThree", 2.000, 3.000, 4.000));
inputVariable->addTerm(new fl::Triangle("AboutFour", 3.000, 4.000, 5.000));
inputVariable->addTerm(new fl::Triangle("AboutFive", 4.000, 5.000, 6.000));
engine->addInputVariable(inputVariable);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("ApproxXCubed");
outputVariable->setRange(-5.000, 5.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(fl::Linear::create("TangentatNegFive", engine->inputVariables(), 75.000, 250.000));
outputVariable->addTerm(fl::Linear::create("TangentatNegFour", engine->inputVariables(), 48.000, 128.000));
outputVariable->addTerm(fl::Linear::create("TangentatNegThree", engine->inputVariables(), 27.000, 54.000));
outputVariable->addTerm(fl::Linear::create("TangentatNegTwo", engine->inputVariables(), 12.000, 16.000));
outputVariable->addTerm(fl::Linear::create("TangentatNegOne", engine->inputVariables(), 3.000, 2.000));
outputVariable->addTerm(fl::Linear::create("TangentatZero", engine->inputVariables(), 0.000, 0.000));
outputVariable->addTerm(fl::Linear::create("TangentatOne", engine->inputVariables(), 3.000, -2.000));
outputVariable->addTerm(fl::Linear::create("TangentatTwo", engine->inputVariables(), 12.000, -16.000));
outputVariable->addTerm(fl::Linear::create("TangentatThree", engine->inputVariables(), 27.000, -54.000));
outputVariable->addTerm(fl::Linear::create("TangentatFour", engine->inputVariables(), 48.000, -128.000));
outputVariable->addTerm(fl::Linear::create("TangentatFive", engine->inputVariables(), 75.000, -250.000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if X is AboutNegFive then ApproxXCubed is TangentatNegFive", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutNegFour then ApproxXCubed is TangentatNegFour", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutNegThree then ApproxXCubed is TangentatNegThree", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutNegTwo then ApproxXCubed is TangentatNegTwo", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutNegOne then ApproxXCubed is TangentatNegOne", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutZero then ApproxXCubed is TangentatZero", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutOne then ApproxXCubed is TangentatOne", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutTwo then ApproxXCubed is TangentatTwo", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutThree then ApproxXCubed is TangentatThree", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutFour then ApproxXCubed is TangentatFour", engine));
ruleBlock->addRule(fl::Rule::parse("if X is AboutFive then ApproxXCubed is TangentatFive", engine));
engine->addRuleBlock(ruleBlock);


}
