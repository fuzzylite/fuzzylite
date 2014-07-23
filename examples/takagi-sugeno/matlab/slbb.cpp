#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("slbb");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("in1");
inputVariable1->setRange(-1.500, 1.500);
inputVariable1->addTerm(new Bell("in1mf1", -1.500, 1.500, 2.000));
inputVariable1->addTerm(new Bell("in1mf2", 1.500, 1.500, 2.000));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("in2");
inputVariable2->setRange(-1.500, 1.500);
inputVariable2->addTerm(new Bell("in2mf1", -1.500, 1.500, 2.000));
inputVariable2->addTerm(new Bell("in2mf2", 1.500, 1.500, 2.000));
engine->addInputVariable(inputVariable2);

InputVariable* inputVariable3 = new InputVariable;
inputVariable3->setEnabled(true);
inputVariable3->setName("in3");
inputVariable3->setRange(-0.200, 0.200);
inputVariable3->addTerm(new Bell("in3mf1", -0.200, 0.200, 2.000));
inputVariable3->addTerm(new Bell("in3mf2", 0.200, 0.200, 2.000));
engine->addInputVariable(inputVariable3);

InputVariable* inputVariable4 = new InputVariable;
inputVariable4->setEnabled(true);
inputVariable4->setName("in4");
inputVariable4->setRange(-0.400, 0.400);
inputVariable4->addTerm(new Bell("in4mf1", -0.400, 0.400, 2.000));
inputVariable4->addTerm(new Bell("in4mf2", 0.400, 0.400, 2.000));
engine->addInputVariable(inputVariable4);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("out");
outputVariable->setRange(-10.000, 10.000);
outputVariable->fuzzyOutput()->setAccumulation(fl::null);
outputVariable->setDefuzzifier(new WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(Linear::create("outmf1", engine, 1.015, 2.234, -12.665, -4.046, 0.026));
outputVariable->addTerm(Linear::create("outmf2", engine, 1.161, 1.969, -9.396, -6.165, 0.474));
outputVariable->addTerm(Linear::create("outmf3", engine, 1.506, 2.234, -12.990, -1.865, 1.426));
outputVariable->addTerm(Linear::create("outmf4", engine, 0.734, 1.969, -9.381, -4.688, -0.880));
outputVariable->addTerm(Linear::create("outmf5", engine, 0.734, 2.234, -12.853, -6.110, -1.034));
outputVariable->addTerm(Linear::create("outmf6", engine, 1.413, 1.969, -9.485, -6.592, 1.159));
outputVariable->addTerm(Linear::create("outmf7", engine, 1.225, 2.234, -12.801, -3.929, 0.366));
outputVariable->addTerm(Linear::create("outmf8", engine, 0.985, 1.969, -9.291, -5.115, -0.195));
outputVariable->addTerm(Linear::create("outmf9", engine, 0.985, 1.969, -9.292, -5.115, 0.195));
outputVariable->addTerm(Linear::create("outmf10", engine, 1.225, 2.234, -12.802, -3.929, -0.366));
outputVariable->addTerm(Linear::create("outmf11", engine, 1.413, 1.969, -9.485, -6.592, -1.159));
outputVariable->addTerm(Linear::create("outmf12", engine, 0.734, 2.234, -12.853, -6.110, 1.034));
outputVariable->addTerm(Linear::create("outmf13", engine, 0.734, 1.969, -9.381, -4.688, 0.880));
outputVariable->addTerm(Linear::create("outmf14", engine, 1.506, 2.234, -12.990, -1.865, -1.426));
outputVariable->addTerm(Linear::create("outmf15", engine, 1.161, 1.969, -9.396, -6.165, -0.474));
outputVariable->addTerm(Linear::create("outmf16", engine, 1.015, 2.234, -12.665, -4.046, -0.026));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new AlgebraicProduct);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf1", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf2", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf3", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf4", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf5", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf6", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf7", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf1 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf8", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf1 then out is outmf9", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf1 and in4 is in4mf2 then out is outmf10", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf1 then out is outmf11", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf1 and in3 is in3mf2 and in4 is in4mf2 then out is outmf12", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf1 then out is outmf13", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf1 and in4 is in4mf2 then out is outmf14", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf1 then out is outmf15", engine));
ruleBlock->addRule(fl::Rule::parse("if in1 is in1mf2 and in2 is in2mf2 and in3 is in3mf2 and in4 is in4mf2 then out is outmf16", engine));
engine->addRuleBlock(ruleBlock);


}
