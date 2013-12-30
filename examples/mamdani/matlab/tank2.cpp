#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("tank");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("level");
inputVariable1->setRange(-1.000, 1.000);
inputVariable1->addTerm(new fl::Trapezoid("high", -2.000, -1.000, -0.800, -0.001));
inputVariable1->addTerm(new fl::Triangle("good", -0.150, 0.000, 0.500));
inputVariable1->addTerm(new fl::Trapezoid("low", 0.001, 0.800, 1.000, 1.500));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("change");
inputVariable2->setRange(-0.100, 0.100);
inputVariable2->addTerm(new fl::Trapezoid("falling", -0.140, -0.100, -0.060, 0.000));
inputVariable2->addTerm(new fl::Trapezoid("rising", -0.001, 0.060, 0.100, 0.140));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("valve");
outputVariable->setRange(-1.000, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(new fl::Triangle("close_fast", -1.000, -0.900, -0.800));
outputVariable->addTerm(new fl::Triangle("close_slow", -0.600, -0.500, -0.400));
outputVariable->addTerm(new fl::Triangle("no_change", -0.100, 0.000, 0.100));
outputVariable->addTerm(new fl::Triangle("open_slow", 0.400, 0.500, 0.600));
outputVariable->addTerm(new fl::Triangle("open_fast", 0.800, 0.900, 1.000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::AlgebraicSum);
ruleBlock->setActivation(new fl::AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if level is low then valve is open_fast", engine));
ruleBlock->addRule(fl::Rule::parse("if level is high then valve is close_fast", engine));
ruleBlock->addRule(fl::Rule::parse("if level is good and change is rising then valve is close_slow", engine));
ruleBlock->addRule(fl::Rule::parse("if level is good and change is falling then valve is open_slow", engine));
engine->addRuleBlock(ruleBlock);


}
