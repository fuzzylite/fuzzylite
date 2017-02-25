#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("slbb");
engine->setDescription("");

InputVariable* in1 = new InputVariable;
in1->setName("in1");
in1->setDescription("");
in1->setEnabled(true);
in1->setRange(-1.500, 1.500);
in1->setLockValueInRange(false);
in1->addTerm(new Bell("in1mf1", -1.500, 1.500, 2.000));
in1->addTerm(new Bell("in1mf2", 1.500, 1.500, 2.000));
engine->addInputVariable(in1);

InputVariable* in2 = new InputVariable;
in2->setName("in2");
in2->setDescription("");
in2->setEnabled(true);
in2->setRange(-1.500, 1.500);
in2->setLockValueInRange(false);
in2->addTerm(new Bell("in2mf1", -1.500, 1.500, 2.000));
in2->addTerm(new Bell("in2mf2", 1.500, 1.500, 2.000));
engine->addInputVariable(in2);

InputVariable* in3 = new InputVariable;
in3->setName("in3");
in3->setDescription("");
in3->setEnabled(true);
in3->setRange(-0.200, 0.200);
in3->setLockValueInRange(false);
in3->addTerm(new Bell("in3mf1", -0.200, 0.200, 2.000));
in3->addTerm(new Bell("in3mf2", 0.200, 0.200, 2.000));
engine->addInputVariable(in3);

InputVariable* in4 = new InputVariable;
in4->setName("in4");
in4->setDescription("");
in4->setEnabled(true);
in4->setRange(-0.400, 0.400);
in4->setLockValueInRange(false);
in4->addTerm(new Bell("in4mf1", -0.400, 0.400, 2.000));
in4->addTerm(new Bell("in4mf2", 0.400, 0.400, 2.000));
engine->addInputVariable(in4);

OutputVariable* out = new OutputVariable;
out->setName("out");
out->setDescription("");
out->setEnabled(true);
out->setRange(-10.000, 10.000);
out->setLockValueInRange(false);
out->setAggregation(fl::null);
out->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
out->setDefaultValue(fl::nan);
out->setLockPreviousValue(false);
out->addTerm(Linear::create("outmf1", engine, 1.015, 2.234, -12.665, -4.046, 0.026));
out->addTerm(Linear::create("outmf2", engine, 1.161, 1.969, -9.396, -6.165, 0.474));
out->addTerm(Linear::create("outmf3", engine, 1.506, 2.234, -12.990, -1.865, 1.426));
out->addTerm(Linear::create("outmf4", engine, 0.734, 1.969, -9.381, -4.688, -0.880));
out->addTerm(Linear::create("outmf5", engine, 0.734, 2.234, -12.853, -6.110, -1.034));
out->addTerm(Linear::create("outmf6", engine, 1.413, 1.969, -9.485, -6.592, 1.159));
out->addTerm(Linear::create("outmf7", engine, 1.225, 2.234, -12.801, -3.929, 0.366));
out->addTerm(Linear::create("outmf8", engine, 0.985, 1.969, -9.291, -5.115, -0.195));
out->addTerm(Linear::create("outmf9", engine, 0.985, 1.969, -9.292, -5.115, 0.195));
out->addTerm(Linear::create("outmf10", engine, 1.225, 2.234, -12.802, -3.929, -0.366));
out->addTerm(Linear::create("outmf11", engine, 1.413, 1.969, -9.485, -6.592, -1.159));
out->addTerm(Linear::create("outmf12", engine, 0.734, 2.234, -12.853, -6.110, 1.034));
out->addTerm(Linear::create("outmf13", engine, 0.734, 1.969, -9.381, -4.688, 0.880));
out->addTerm(Linear::create("outmf14", engine, 1.506, 2.234, -12.990, -1.865, -1.426));
out->addTerm(Linear::create("outmf15", engine, 1.161, 1.969, -9.396, -6.165, -0.474));
out->addTerm(Linear::create("outmf16", engine, 1.015, 2.234, -12.665, -4.046, -0.026));
engine->addOutputVariable(out);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new AlgebraicProduct);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf1", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf2", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf3", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf4", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf5", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf6", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf7", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf8", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf9", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf10", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf11", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf12", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf13", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf14", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf15", engine));
ruleBlock->addRule(Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf16", engine));
engine->addRuleBlock(ruleBlock);


}
