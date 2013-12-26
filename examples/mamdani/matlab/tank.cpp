#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("tank");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("level");
inputVariable1->setRange(-1.00000000, 1.00000000);
inputVariable1->addTerm(new fl::Gaussian("high", -1.00000000, 0.30000000));
inputVariable1->addTerm(new fl::Gaussian("okay", 0.00000000, 0.30000000));
inputVariable1->addTerm(new fl::Gaussian("low", 1.00000000, 0.30000000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("rate");
inputVariable2->setRange(-0.10000000, 0.10000000);
inputVariable2->addTerm(new fl::Gaussian("negative", -0.10000000, 0.03000000));
inputVariable2->addTerm(new fl::Gaussian("none", 0.00000000, 0.03000000));
inputVariable2->addTerm(new fl::Gaussian("positive", 0.10000000, 0.03000000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("valve");
outputVariable->setRange(-1.00000000, 1.00000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(new fl::Triangle("close_fast", -1.00000000, -0.90000000, -0.80000000));
outputVariable->addTerm(new fl::Triangle("close_slow", -0.60000000, -0.50000000, -0.40000000));
outputVariable->addTerm(new fl::Triangle("no_change", -0.10000000, 0.00000000, 0.10000000));
outputVariable->addTerm(new fl::Triangle("open_slow", 0.20000000, 0.30000000, 0.40000000));
outputVariable->addTerm(new fl::Triangle("open_fast", 0.80000000, 0.90000000, 1.00000000));
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
