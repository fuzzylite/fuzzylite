#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("");

InputVariable* inputVariable = new InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("X");
inputVariable->setRange(-10.000, 10.000);
inputVariable->addTerm(new Bell("small", -10.000, 5.000, 3.000));
inputVariable->addTerm(new Bell("medium", 0.000, 5.000, 3.000));
inputVariable->addTerm(new Bell("large", 10.000, 5.000, 3.000));
engine->addInputVariable(inputVariable);

OutputVariable* outputVariable1 = new OutputVariable;
outputVariable1->setEnabled(true);
outputVariable1->setName("Ramps");
outputVariable1->setRange(0.000, 1.000);
outputVariable1->fuzzyOutput()->setAccumulation(fl::null);
outputVariable1->setDefuzzifier(new WeightedAverage);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockPreviousOutputValue(false);
outputVariable1->setLockOutputValueInRange(false);
outputVariable1->addTerm(new Ramp("b", 0.600, 0.400));
outputVariable1->addTerm(new Ramp("a", 0.000, 0.250));
outputVariable1->addTerm(new Ramp("c", 0.700, 1.000));
engine->addOutputVariable(outputVariable1);

OutputVariable* outputVariable2 = new OutputVariable;
outputVariable2->setEnabled(true);
outputVariable2->setName("Sigmoids");
outputVariable2->setRange(0.020, 1.000);
outputVariable2->fuzzyOutput()->setAccumulation(fl::null);
outputVariable2->setDefuzzifier(new WeightedAverage);
outputVariable2->setDefaultValue(fl::nan);
outputVariable2->setLockPreviousOutputValue(false);
outputVariable2->setLockOutputValueInRange(false);
outputVariable2->addTerm(new Sigmoid("b", 0.500, -30.000));
outputVariable2->addTerm(new Sigmoid("a", 0.130, 30.000));
outputVariable2->addTerm(new Sigmoid("c", 0.830, 30.000));
engine->addOutputVariable(outputVariable2);

OutputVariable* outputVariable3 = new OutputVariable;
outputVariable3->setEnabled(true);
outputVariable3->setName("ZSShapes");
outputVariable3->setRange(0.000, 1.000);
outputVariable3->fuzzyOutput()->setAccumulation(fl::null);
outputVariable3->setDefuzzifier(new WeightedAverage);
outputVariable3->setDefaultValue(fl::nan);
outputVariable3->setLockPreviousOutputValue(false);
outputVariable3->setLockOutputValueInRange(false);
outputVariable3->addTerm(new ZShape("b", 0.300, 0.600));
outputVariable3->addTerm(new SShape("a", 0.000, 0.250));
outputVariable3->addTerm(new SShape("c", 0.700, 1.000));
engine->addOutputVariable(outputVariable3);

OutputVariable* outputVariable4 = new OutputVariable;
outputVariable4->setEnabled(true);
outputVariable4->setName("Concaves");
outputVariable4->setRange(0.000, 1.000);
outputVariable4->fuzzyOutput()->setAccumulation(fl::null);
outputVariable4->setDefuzzifier(new WeightedAverage);
outputVariable4->setDefaultValue(fl::nan);
outputVariable4->setLockPreviousOutputValue(false);
outputVariable4->setLockOutputValueInRange(false);
outputVariable4->addTerm(new Concave("b", 0.500, 0.400));
outputVariable4->addTerm(new Concave("a", 0.240, 0.250));
outputVariable4->addTerm(new Concave("c", 0.900, 1.000));
engine->addOutputVariable(outputVariable4);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if X is small then Ramps is a and Sigmoids is a and ZSShapes is a and Concaves is a", engine));
ruleBlock->addRule(fl::Rule::parse("if X is medium then Ramps is b and Sigmoids is b and ZSShapes is b and Concaves is b", engine));
ruleBlock->addRule(fl::Rule::parse("if X is large then Ramps is c and Sigmoids is c and ZSShapes is c and Concaves is c", engine));
engine->addRuleBlock(ruleBlock);


}
