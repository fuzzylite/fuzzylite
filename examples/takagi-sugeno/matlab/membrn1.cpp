#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("anfis");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("in_n1");
inputVariable1->setRange(1.000, 31.000);
inputVariable1->addTerm(new Bell("in1mf1", 2.253, 16.220, 5.050));
inputVariable1->addTerm(new Bell("in1mf2", 31.260, 15.021, 1.843));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("in_n2");
inputVariable2->setRange(1.000, 31.000);
inputVariable2->addTerm(new Bell("in2mf1", 0.740, 15.021, 1.843));
inputVariable2->addTerm(new Bell("in2mf2", 29.747, 16.220, 5.050));
engine->addInputVariable(inputVariable2);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("out1");
outputVariable->setRange(-0.334, 1.000);
outputVariable->fuzzyOutput()->setAccumulation(fl::null);
outputVariable->setDefuzzifier(new WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(Linear::create("out1mf1", engine, 0.026, 0.071, -0.615));
outputVariable->addTerm(Linear::create("out1mf2", engine, -0.036, 0.036, -1.169));
outputVariable->addTerm(Linear::create("out1mf3", engine, -0.094, 0.094, 2.231));
outputVariable->addTerm(Linear::create("out1mf4", engine, -0.071, -0.026, 2.479));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new AlgebraicProduct);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf3", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf4", engine));
engine->addRuleBlock(ruleBlock);


}
