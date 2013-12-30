#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("tanksg1");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("level");
inputVariable1->setRange(-1.000, 1.000);
inputVariable1->addTerm(new fl::Gaussian("high", -1.000, 0.300));
inputVariable1->addTerm(new fl::Gaussian("okay", 0.004, 0.300));
inputVariable1->addTerm(new fl::Gaussian("low", 1.000, 0.300));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("rate");
inputVariable2->setRange(-0.100, 0.100);
inputVariable2->addTerm(new fl::Gaussian("negative", -0.100, 0.030));
inputVariable2->addTerm(new fl::Gaussian("none", 0.000, 0.030));
inputVariable2->addTerm(new fl::Gaussian("positive", 0.100, 0.030));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("valve");
outputVariable->setRange(-1.000, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(new fl::Constant("close_fast", -0.900));
outputVariable->addTerm(new fl::Constant("close_slow", -0.500));
outputVariable->addTerm(new fl::Constant("no_change", 0.000));
outputVariable->addTerm(new fl::Constant("open_slow", 0.300));
outputVariable->addTerm(new fl::Constant("open_fast", 0.900));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::AlgebraicSum);
ruleBlock->setActivation(new fl::AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if level is okay then valve is no_change", engine));
ruleBlock->addRule(fl::Rule::parse("if level is low then valve is open_fast", engine));
ruleBlock->addRule(fl::Rule::parse("if level is high then valve is close_fast", engine));
ruleBlock->addRule(fl::Rule::parse("if level is okay and rate is positive then valve is close_slow", engine));
ruleBlock->addRule(fl::Rule::parse("if level is okay and rate is negative then valve is open_slow", engine));
engine->addRuleBlock(ruleBlock);


}
