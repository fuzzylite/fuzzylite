#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("membrn1");
engine->setDescription("");

InputVariable* in_n1 = new InputVariable;
in_n1->setName("in_n1");
in_n1->setDescription("");
in_n1->setEnabled(true);
in_n1->setRange(1.000, 31.000);
in_n1->setLockValueInRange(false);
in_n1->addTerm(new Bell("in1mf1", 2.253, 16.220, 5.050));
in_n1->addTerm(new Bell("in1mf2", 31.260, 15.021, 1.843));
engine->addInputVariable(in_n1);

InputVariable* in_n2 = new InputVariable;
in_n2->setName("in_n2");
in_n2->setDescription("");
in_n2->setEnabled(true);
in_n2->setRange(1.000, 31.000);
in_n2->setLockValueInRange(false);
in_n2->addTerm(new Bell("in2mf1", 0.740, 15.021, 1.843));
in_n2->addTerm(new Bell("in2mf2", 29.747, 16.220, 5.050));
engine->addInputVariable(in_n2);

OutputVariable* out1 = new OutputVariable;
out1->setName("out1");
out1->setDescription("");
out1->setEnabled(true);
out1->setRange(-0.334, 1.000);
out1->setLockValueInRange(false);
out1->setAggregation(fl::null);
out1->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
out1->setDefaultValue(fl::nan);
out1->setLockPreviousValue(false);
out1->addTerm(Linear::create("out1mf1", engine, 0.026, 0.071, -0.615));
out1->addTerm(Linear::create("out1mf2", engine, -0.036, 0.036, -1.169));
out1->addTerm(Linear::create("out1mf3", engine, -0.094, 0.094, 2.231));
out1->addTerm(Linear::create("out1mf4", engine, -0.071, -0.026, 2.479));
engine->addOutputVariable(out1);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new AlgebraicProduct);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1", engine));
ruleBlock->addRule(Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2", engine));
ruleBlock->addRule(Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf3", engine));
ruleBlock->addRule(Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf4", engine));
engine->addRuleBlock(ruleBlock);


}
