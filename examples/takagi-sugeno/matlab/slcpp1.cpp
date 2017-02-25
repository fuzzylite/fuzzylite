#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("slcpp1");
engine->setDescription("");

InputVariable* in1 = new InputVariable;
in1->setName("in1");
in1->setDescription("");
in1->setEnabled(true);
in1->setRange(-0.300, 0.300);
in1->setLockValueInRange(false);
engine->addInputVariable(in1);

InputVariable* in2 = new InputVariable;
in2->setName("in2");
in2->setDescription("");
in2->setEnabled(true);
in2->setRange(-1.000, 1.000);
in2->setLockValueInRange(false);
engine->addInputVariable(in2);

InputVariable* in3 = new InputVariable;
in3->setName("in3");
in3->setDescription("");
in3->setEnabled(true);
in3->setRange(-3.000, 3.000);
in3->setLockValueInRange(false);
engine->addInputVariable(in3);

InputVariable* in4 = new InputVariable;
in4->setName("in4");
in4->setDescription("");
in4->setEnabled(true);
in4->setRange(-3.000, 3.000);
in4->setLockValueInRange(false);
engine->addInputVariable(in4);

InputVariable* in5 = new InputVariable;
in5->setName("in5");
in5->setDescription("");
in5->setEnabled(true);
in5->setRange(-3.000, 3.000);
in5->setLockValueInRange(false);
engine->addInputVariable(in5);

InputVariable* in6 = new InputVariable;
in6->setName("in6");
in6->setDescription("");
in6->setEnabled(true);
in6->setRange(-3.000, 3.000);
in6->setLockValueInRange(false);
engine->addInputVariable(in6);

InputVariable* pole_length = new InputVariable;
pole_length->setName("pole_length");
pole_length->setDescription("");
pole_length->setEnabled(true);
pole_length->setRange(0.500, 1.500);
pole_length->setLockValueInRange(false);
pole_length->addTerm(new ZShape("mf1", 0.500, 0.600));
pole_length->addTerm(new PiShape("mf2", 0.500, 0.600, 0.600, 0.700));
pole_length->addTerm(new PiShape("mf3", 0.600, 0.700, 0.700, 0.800));
pole_length->addTerm(new PiShape("mf4", 0.700, 0.800, 0.800, 0.900));
pole_length->addTerm(new PiShape("mf5", 0.800, 0.900, 0.900, 1.000));
pole_length->addTerm(new PiShape("mf6", 0.900, 1.000, 1.000, 1.100));
pole_length->addTerm(new PiShape("mf7", 1.000, 1.100, 1.100, 1.200));
pole_length->addTerm(new PiShape("mf8", 1.100, 1.200, 1.200, 1.300));
pole_length->addTerm(new PiShape("mf9", 1.200, 1.300, 1.300, 1.400));
pole_length->addTerm(new PiShape("mf10", 1.300, 1.400, 1.400, 1.500));
pole_length->addTerm(new SShape("mf11", 1.400, 1.500));
engine->addInputVariable(pole_length);

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
out->addTerm(Linear::create("outmf1", engine, 168.400, 31.000, -188.050, -49.250, -1.000, -2.700, 0.000, 0.000));
out->addTerm(Linear::create("outmf2", engine, 233.950, 47.190, -254.520, -66.580, -1.000, -2.740, 0.000, 0.000));
out->addTerm(Linear::create("outmf3", engine, 342.940, 74.730, -364.370, -95.230, -1.000, -2.780, 0.000, 0.000));
out->addTerm(Linear::create("outmf4", engine, 560.710, 130.670, -582.960, -152.240, -1.000, -2.810, 0.000, 0.000));
out->addTerm(Linear::create("outmf5", engine, 1213.880, 300.190, -1236.900, -322.800, -1.000, -2.840, 0.000, 0.000));
out->addTerm(Linear::create("outmf6", engine, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000));
out->addTerm(Linear::create("outmf7", engine, -1399.120, -382.950, 1374.630, 358.340, -1.000, -2.900, 0.000, 0.000));
out->addTerm(Linear::create("outmf8", engine, -746.070, -213.420, 720.900, 187.840, -1.000, -2.930, 0.000, 0.000));
out->addTerm(Linear::create("outmf9", engine, -528.520, -157.460, 502.680, 130.920, -1.000, -2.960, 0.000, 0.000));
out->addTerm(Linear::create("outmf10", engine, -419.870, -129.890, 393.380, 102.410, -1.000, -2.980, 0.000, 0.000));
out->addTerm(Linear::create("outmf11", engine, -354.770, -113.680, 327.650, 85.270, -1.000, -3.010, 0.000, 0.000));
engine->addOutputVariable(out);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if pole_length is mf1 then out is outmf1", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf2 then out is outmf2", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf3 then out is outmf3", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf4 then out is outmf4", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf5 then out is outmf5", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf6 then out is outmf6", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf7 then out is outmf7", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf8 then out is outmf8", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf9 then out is outmf9", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf10 then out is outmf10", engine));
ruleBlock->addRule(Rule::parse("if pole_length is mf11 then out is outmf11", engine));
engine->addRuleBlock(ruleBlock);


}
