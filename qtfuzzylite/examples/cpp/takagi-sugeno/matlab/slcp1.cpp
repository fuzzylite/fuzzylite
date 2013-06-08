#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("slcp1");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("in1");
inputVariable1->setRange(-0.300, 0.300);

engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("in2");
inputVariable2->setRange(-1.000, 1.000);

engine->addInputVariable(inputVariable2);

fl::InputVariable* inputVariable3 = new fl::InputVariable;
inputVariable3->setName("in3");
inputVariable3->setRange(-3.000, 3.000);

engine->addInputVariable(inputVariable3);

fl::InputVariable* inputVariable4 = new fl::InputVariable;
inputVariable4->setName("in4");
inputVariable4->setRange(-3.000, 3.000);

engine->addInputVariable(inputVariable4);

fl::InputVariable* inputVariable5 = new fl::InputVariable;
inputVariable5->setName("in5");
inputVariable5->setRange(0.500, 1.500);

inputVariable5->addTerm(new fl::Gaussian("small", 0.500, 0.200));
inputVariable5->addTerm(new fl::Gaussian("medium", 1.000, 0.200));
inputVariable5->addTerm(new fl::Gaussian("large", 1.500, 0.200));
engine->addInputVariable(inputVariable5);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("out");
outputVariable1->setRange(-10.000, 10.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("outmf1", engine->inputVariables(), 32.166, 5.835, 3.162, 3.757, 0.000, 0.000));
outputVariable1->addTerm(fl::Linear::create("outmf2", engine->inputVariables(), 39.012, 9.947, 3.162, 4.269, 0.000, 0.000));
outputVariable1->addTerm(fl::Linear::create("outmf3", engine->inputVariables(), 45.009, 13.985, 3.162, 4.666, 0.000, 0.000));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::AlgebraicProduct);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::AlgebraicProduct);

ruleblock1->addRule(fl::FuzzyRule::parse("if in5 is small then out is outmf1", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in5 is medium then out is outmf2", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if in5 is large then out is outmf3", engine));
engine->addRuleBlock(ruleblock1);


}
