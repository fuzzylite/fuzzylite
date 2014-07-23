#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("peaks");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("in1");
inputVariable1->setRange(-3.000, 3.000);
inputVariable1->addTerm(new Bell("in1mf1", -2.233, 1.578, 2.151));
inputVariable1->addTerm(new Bell("in1mf2", -0.394, 0.753, 1.838));
inputVariable1->addTerm(new Bell("in1mf3", 0.497, 0.689, 1.844));
inputVariable1->addTerm(new Bell("in1mf4", 2.270, 1.528, 2.156));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("in2");
inputVariable2->setRange(-3.000, 3.000);
inputVariable2->addTerm(new Bell("in1mf1", -2.686, 1.267, 2.044));
inputVariable2->addTerm(new Bell("in1mf2", -0.836, 1.266, 1.796));
inputVariable2->addTerm(new Bell("in1mf3", 0.859, 1.314, 1.937));
inputVariable2->addTerm(new Bell("in1mf4", 2.727, 1.214, 2.047));
engine->addInputVariable(inputVariable2);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("out1");
outputVariable->setRange(-10.000, 10.000);
outputVariable->fuzzyOutput()->setAccumulation(fl::null);
outputVariable->setDefuzzifier(new WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(Linear::create("out1mf1", engine, 0.155, -2.228, -8.974));
outputVariable->addTerm(Linear::create("out1mf2", engine, -0.312, -7.705, -9.055));
outputVariable->addTerm(Linear::create("out1mf3", engine, -0.454, -4.437, 6.930));
outputVariable->addTerm(Linear::create("out1mf4", engine, 0.248, -1.122, 5.081));
outputVariable->addTerm(Linear::create("out1mf5", engine, -6.278, 25.211, 99.148));
outputVariable->addTerm(Linear::create("out1mf6", engine, 5.531, 105.916, 157.283));
outputVariable->addTerm(Linear::create("out1mf7", engine, 19.519, 112.333, -127.796));
outputVariable->addTerm(Linear::create("out1mf8", engine, -5.079, 34.738, -143.414));
outputVariable->addTerm(Linear::create("out1mf9", engine, -5.889, 27.311, 116.585));
outputVariable->addTerm(Linear::create("out1mf10", engine, 21.517, 97.266, 93.802));
outputVariable->addTerm(Linear::create("out1mf11", engine, 9.198, 79.853, -118.482));
outputVariable->addTerm(Linear::create("out1mf12", engine, -6.571, 23.026, -87.747));
outputVariable->addTerm(Linear::create("out1mf13", engine, 0.092, -1.126, -4.527));
outputVariable->addTerm(Linear::create("out1mf14", engine, -0.304, -4.434, -6.561));
outputVariable->addTerm(Linear::create("out1mf15", engine, -0.166, -6.284, 7.307));
outputVariable->addTerm(Linear::create("out1mf16", engine, 0.107, -2.028, 8.159));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new AlgebraicProduct);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in1mf1 then out1 is out1mf1", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in1mf2 then out1 is out1mf2", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in1mf3 then out1 is out1mf3", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in1mf4 then out1 is out1mf4", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in1mf1 then out1 is out1mf5", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in1mf2 then out1 is out1mf6", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in1mf3 then out1 is out1mf7", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in1mf4 then out1 is out1mf8", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf3 and in2 is in1mf1 then out1 is out1mf9", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf3 and in2 is in1mf2 then out1 is out1mf10", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf3 and in2 is in1mf3 then out1 is out1mf11", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf3 and in2 is in1mf4 then out1 is out1mf12", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf4 and in2 is in1mf1 then out1 is out1mf13", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf4 and in2 is in1mf2 then out1 is out1mf14", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf4 and in2 is in1mf3 then out1 is out1mf15", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf4 and in2 is in1mf4 then out1 is out1mf16", engine));
engine->addRuleBlock(ruleBlock);


}
