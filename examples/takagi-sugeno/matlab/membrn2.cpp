#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("anfis");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("in_n1");
inputVariable1->setRange(1.00000000, 31.00000000);
inputVariable1->addTerm(new fl::Bell("in1mf1", 1.15200000, 8.20600000, 0.87400000));
inputVariable1->addTerm(new fl::Bell("in1mf2", 15.88200000, 7.07800000, 0.44400000));
inputVariable1->addTerm(new fl::Bell("in1mf3", 30.57500000, 8.60200000, 0.81800000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("in_n2");
inputVariable2->setRange(1.00000000, 31.00000000);
inputVariable2->addTerm(new fl::Bell("in2mf1", 1.42600000, 8.60200000, 0.81800000));
inputVariable2->addTerm(new fl::Bell("in2mf2", 16.11800000, 7.07800000, 0.44500000));
inputVariable2->addTerm(new fl::Bell("in2mf3", 30.84700000, 8.20600000, 0.87500000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("out1");
outputVariable->setRange(-0.33400000, 1.00000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(fl::Linear::create("out1mf1", engine->inputVariables(), -0.03500000, 0.00200000, -0.35200000));
outputVariable->addTerm(fl::Linear::create("out1mf2", engine->inputVariables(), 0.04400000, 0.07900000, -0.02800000));
outputVariable->addTerm(fl::Linear::create("out1mf3", engine->inputVariables(), -0.02400000, 0.02400000, -1.59900000));
outputVariable->addTerm(fl::Linear::create("out1mf4", engine->inputVariables(), -0.06700000, 0.38400000, 0.00700000));
outputVariable->addTerm(fl::Linear::create("out1mf5", engine->inputVariables(), 0.35100000, -0.35100000, -3.66300000));
outputVariable->addTerm(fl::Linear::create("out1mf6", engine->inputVariables(), -0.07900000, -0.04400000, 3.90900000));
outputVariable->addTerm(fl::Linear::create("out1mf7", engine->inputVariables(), 0.01200000, -0.01200000, -0.60000000));
outputVariable->addTerm(fl::Linear::create("out1mf8", engine->inputVariables(), -0.38400000, 0.06700000, 10.15800000));
outputVariable->addTerm(fl::Linear::create("out1mf9", engine->inputVariables(), -0.00200000, 0.03500000, -1.40200000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf1 then out1 is out1mf1", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf2 then out1 is out1mf2", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf1 and in_n2 is in2mf3 then out1 is out1mf3", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf1 then out1 is out1mf4", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf2 then out1 is out1mf5", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf2 and in_n2 is in2mf3 then out1 is out1mf6", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf3 and in_n2 is in2mf1 then out1 is out1mf7", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf3 and in_n2 is in2mf2 then out1 is out1mf8", engine));
ruleBlock->addRule(fl::Rule::parse("if in_n1 is in1mf3 and in_n2 is in2mf3 then out1 is out1mf9", engine));
engine->addRuleBlock(ruleBlock);


}
