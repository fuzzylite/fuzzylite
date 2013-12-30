#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("juggler");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("xHit");
inputVariable1->setRange(-4.000, 4.000);
inputVariable1->addTerm(new fl::Bell("in1mf1", -4.000, 2.000, 4.000));
inputVariable1->addTerm(new fl::Bell("in1mf2", 0.000, 2.000, 4.000));
inputVariable1->addTerm(new fl::Bell("in1mf3", 4.000, 2.000, 4.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("projectAngle");
inputVariable2->setRange(0.000, 3.142);
inputVariable2->addTerm(new fl::Bell("in2mf1", 0.000, 0.785, 4.000));
inputVariable2->addTerm(new fl::Bell("in2mf2", 1.571, 0.785, 4.000));
inputVariable2->addTerm(new fl::Bell("in2mf3", 3.142, 0.785, 4.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("theta");
outputVariable->setRange(0.000, 0.000);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 0.315));
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 0.315));
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 0.315));
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), 0.114, -0.500, 0.785));
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), 0.114, -0.500, 0.785));
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), 0.114, -0.500, 0.785));
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 1.256));
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 1.256));
outputVariable->addTerm(fl::Linear::create("out1mf", engine->inputVariables(), -0.022, -0.500, 1.256));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf1 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf1 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf1 and projectAngle is in2mf3 then theta is out1mf", engine));
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf2 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf2 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf2 and projectAngle is in2mf3 then theta is out1mf", engine));
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf3 and projectAngle is in2mf1 then theta is out1mf", engine));
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf3 and projectAngle is in2mf2 then theta is out1mf", engine));
ruleBlock->addRule(fl::Rule::parse("if xHit is in1mf3 and projectAngle is in2mf3 then theta is out1mf", engine));
engine->addRuleBlock(ruleBlock);


}
