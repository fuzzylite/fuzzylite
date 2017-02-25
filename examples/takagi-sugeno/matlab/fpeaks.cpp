#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("fpeaks");
engine->setDescription("");

InputVariable* in1 = new InputVariable;
in1->setName("in1");
in1->setDescription("");
in1->setEnabled(true);
in1->setRange(-3.000, 3.000);
in1->setLockValueInRange(false);
in1->addTerm(new Bell("in1mf1", -2.233, 1.578, 2.151));
in1->addTerm(new Bell("in1mf2", -0.394, 0.753, 1.838));
in1->addTerm(new Bell("in1mf3", 0.497, 0.689, 1.844));
in1->addTerm(new Bell("in1mf4", 2.270, 1.528, 2.156));
engine->addInputVariable(in1);

InputVariable* in2 = new InputVariable;
in2->setName("in2");
in2->setDescription("");
in2->setEnabled(true);
in2->setRange(-3.000, 3.000);
in2->setLockValueInRange(false);
in2->addTerm(new Bell("in1mf1", -2.686, 1.267, 2.044));
in2->addTerm(new Bell("in1mf2", -0.836, 1.266, 1.796));
in2->addTerm(new Bell("in1mf3", 0.859, 1.314, 1.937));
in2->addTerm(new Bell("in1mf4", 2.727, 1.214, 2.047));
engine->addInputVariable(in2);

OutputVariable* out1 = new OutputVariable;
out1->setName("out1");
out1->setDescription("");
out1->setEnabled(true);
out1->setRange(-10.000, 10.000);
out1->setLockValueInRange(false);
out1->setAggregation(fl::null);
out1->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
out1->setDefaultValue(fl::nan);
out1->setLockPreviousValue(false);
out1->addTerm(Linear::create("out1mf1", engine, 0.155, -2.228, -8.974));
out1->addTerm(Linear::create("out1mf2", engine, -0.312, -7.705, -9.055));
out1->addTerm(Linear::create("out1mf3", engine, -0.454, -4.437, 6.930));
out1->addTerm(Linear::create("out1mf4", engine, 0.248, -1.122, 5.081));
out1->addTerm(Linear::create("out1mf5", engine, -6.278, 25.211, 99.148));
out1->addTerm(Linear::create("out1mf6", engine, 5.531, 105.916, 157.283));
out1->addTerm(Linear::create("out1mf7", engine, 19.519, 112.333, -127.796));
out1->addTerm(Linear::create("out1mf8", engine, -5.079, 34.738, -143.414));
out1->addTerm(Linear::create("out1mf9", engine, -5.889, 27.311, 116.585));
out1->addTerm(Linear::create("out1mf10", engine, 21.517, 97.266, 93.802));
out1->addTerm(Linear::create("out1mf11", engine, 9.198, 79.853, -118.482));
out1->addTerm(Linear::create("out1mf12", engine, -6.571, 23.026, -87.747));
out1->addTerm(Linear::create("out1mf13", engine, 0.092, -1.126, -4.527));
out1->addTerm(Linear::create("out1mf14", engine, -0.304, -4.434, -6.561));
out1->addTerm(Linear::create("out1mf15", engine, -0.166, -6.284, 7.307));
out1->addTerm(Linear::create("out1mf16", engine, 0.107, -2.028, 8.159));
engine->addOutputVariable(out1);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new AlgebraicProduct);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in1mf1 then out1 is out1mf1", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in1mf2 then out1 is out1mf2", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in1mf3 then out1 is out1mf3", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in1mf4 then out1 is out1mf4", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in1mf1 then out1 is out1mf5", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in1mf2 then out1 is out1mf6", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in1mf3 then out1 is out1mf7", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in1mf4 then out1 is out1mf8", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf3 and in2 is in1mf1 then out1 is out1mf9", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf3 and in2 is in1mf2 then out1 is out1mf10", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf3 and in2 is in1mf3 then out1 is out1mf11", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf3 and in2 is in1mf4 then out1 is out1mf12", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf4 and in2 is in1mf1 then out1 is out1mf13", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf4 and in2 is in1mf2 then out1 is out1mf14", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf4 and in2 is in1mf3 then out1 is out1mf15", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf4 and in2 is in1mf4 then out1 is out1mf16", engine));
engine->addRuleBlock(ruleBlock);


}
