#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("tank");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("level");
inputVariable1->setRange(-1.000, 1.000);
inputVariable1->addTerm(new Gaussian("high", -1.000, 0.300));
inputVariable1->addTerm(new Gaussian("okay", 0.000, 0.300));
inputVariable1->addTerm(new Gaussian("low", 1.000, 0.300));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("rate");
inputVariable2->setRange(-0.100, 0.100);
inputVariable2->addTerm(new Gaussian("negative", -0.100, 0.030));
inputVariable2->addTerm(new Gaussian("none", 0.000, 0.030));
inputVariable2->addTerm(new Gaussian("positive", 0.100, 0.030));
engine->addInputVariable(inputVariable2);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("valve");
outputVariable->setRange(-1.000, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(new Maximum);
outputVariable->setDefuzzifier(new Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(new Triangle("close_fast", -1.000, -0.900, -0.800));
outputVariable->addTerm(new Triangle("close_slow", -0.600, -0.500, -0.400));
outputVariable->addTerm(new Triangle("no_change", -0.100, 0.000, 0.100));
outputVariable->addTerm(new Triangle("open_slow", 0.200, 0.300, 0.400));
outputVariable->addTerm(new Triangle("open_fast", 0.800, 0.900, 1.000));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new AlgebraicProduct);
ruleBlock->setDisjunction(new AlgebraicSum);
ruleBlock->setActivation(new AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if level is okay then valve is no_change", engine));
ruleBlock->addRule(fl::Rule::parse("if level is low then valve is open_fast", engine));
ruleBlock->addRule(fl::Rule::parse("if level is high then valve is close_fast", engine));
ruleBlock->addRule(fl::Rule::parse("if level is okay and rate is positive then valve is close_slow", engine));
ruleBlock->addRule(fl::Rule::parse("if level is okay and rate is negative then valve is open_slow", engine));
engine->addRuleBlock(ruleBlock);


}
