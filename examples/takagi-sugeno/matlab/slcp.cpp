#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("slcp");
engine->setDescription("");

InputVariable* in1 = new InputVariable;
in1->setName("in1");
in1->setDescription("");
in1->setEnabled(true);
in1->setRange(-0.300, 0.300);
in1->setLockValueInRange(false);
in1->addTerm(new Bell("in1mf1", -0.300, 0.300, 2.000));
in1->addTerm(new Bell("in1mf2", 0.300, 0.300, 2.000));
engine->addInputVariable(in1);

InputVariable* in2 = new InputVariable;
in2->setName("in2");
in2->setDescription("");
in2->setEnabled(true);
in2->setRange(-1.000, 1.000);
in2->setLockValueInRange(false);
in2->addTerm(new Bell("in2mf1", -1.000, 1.000, 2.000));
in2->addTerm(new Bell("in2mf2", 1.000, 1.000, 2.000));
engine->addInputVariable(in2);

InputVariable* in3 = new InputVariable;
in3->setName("in3");
in3->setDescription("");
in3->setEnabled(true);
in3->setRange(-3.000, 3.000);
in3->setLockValueInRange(false);
in3->addTerm(new Bell("in3mf1", -3.000, 3.000, 2.000));
in3->addTerm(new Bell("in3mf2", 3.000, 3.000, 2.000));
engine->addInputVariable(in3);

InputVariable* in4 = new InputVariable;
in4->setName("in4");
in4->setDescription("");
in4->setEnabled(true);
in4->setRange(-3.000, 3.000);
in4->setLockValueInRange(false);
in4->addTerm(new Bell("in4mf1", -3.000, 3.000, 2.000));
in4->addTerm(new Bell("in4mf2", 3.000, 3.000, 2.000));
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
out->addTerm(Linear::create("outmf1", engine, 41.373, 10.030, 3.162, 4.288, 0.339));
out->addTerm(Linear::create("outmf2", engine, 40.409, 10.053, 3.162, 4.288, 0.207));
out->addTerm(Linear::create("outmf3", engine, 41.373, 10.030, 3.162, 4.288, 0.339));
out->addTerm(Linear::create("outmf4", engine, 40.409, 10.053, 3.162, 4.288, 0.207));
out->addTerm(Linear::create("outmf5", engine, 38.561, 10.177, 3.162, 4.288, -0.049));
out->addTerm(Linear::create("outmf6", engine, 37.596, 10.154, 3.162, 4.288, -0.181));
out->addTerm(Linear::create("outmf7", engine, 38.561, 10.177, 3.162, 4.288, -0.049));
out->addTerm(Linear::create("outmf8", engine, 37.596, 10.154, 3.162, 4.288, -0.181));
out->addTerm(Linear::create("outmf9", engine, 37.596, 10.154, 3.162, 4.288, 0.181));
out->addTerm(Linear::create("outmf10", engine, 38.561, 10.177, 3.162, 4.288, 0.049));
out->addTerm(Linear::create("outmf11", engine, 37.596, 10.154, 3.162, 4.288, 0.181));
out->addTerm(Linear::create("outmf12", engine, 38.561, 10.177, 3.162, 4.288, 0.049));
out->addTerm(Linear::create("outmf13", engine, 40.408, 10.053, 3.162, 4.288, -0.207));
out->addTerm(Linear::create("outmf14", engine, 41.373, 10.030, 3.162, 4.288, -0.339));
out->addTerm(Linear::create("outmf15", engine, 40.408, 10.053, 3.162, 4.288, -0.207));
out->addTerm(Linear::create("outmf16", engine, 41.373, 10.030, 3.162, 4.288, -0.339));
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
