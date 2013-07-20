#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("sltbu");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("distance");
inputVariable1->setRange(0.000, 25.000);

inputVariable1->addTerm(new fl::ZShape("near", 1.000, 2.000));
inputVariable1->addTerm(new fl::SShape("far", 1.000, 2.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setName("control1");
inputVariable2->setRange(-0.785, 0.785);

engine->addInputVariable(inputVariable2);

fl::InputVariable* inputVariable3 = new fl::InputVariable;
inputVariable3->setName("control2");
inputVariable3->setRange(-0.785, 0.785);

engine->addInputVariable(inputVariable3);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("control");
outputVariable1->setRange(-0.785, 0.785);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage());
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(fl::Linear::create("out1mf1", engine->inputVariables(), 0.000, 0.000, 1.000, 0.000));
outputVariable1->addTerm(fl::Linear::create("out1mf2", engine->inputVariables(), 0.000, 1.000, 0.000, 0.000));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::AlgebraicProduct);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::AlgebraicProduct);

ruleblock1->addRule(fl::FuzzyRule::parse("if distance is near then control is out1mf1", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if distance is far then control is out1mf2", engine));
engine->addRuleBlock(ruleblock1);


}
