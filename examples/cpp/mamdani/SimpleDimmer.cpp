#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("simple-dimmer");
engine->addHedge(new fl::Any);
engine->addHedge(new fl::Extremely);
engine->addHedge(new fl::Not);
engine->addHedge(new fl::Seldom);
engine->addHedge(new fl::Somewhat);
engine->addHedge(new fl::Very);

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setName("Ambient");
inputVariable1->setRange(0.000, 1.000);

inputVariable1->addTerm(new fl::Triangle("DARK", 0.000, 0.250, 0.500));
inputVariable1->addTerm(new fl::Triangle("MEDIUM", 0.250, 0.500, 0.750));
inputVariable1->addTerm(new fl::Triangle("BRIGHT", 0.500, 0.750, 1.000));
engine->addInputVariable(inputVariable1);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setName("Power");
outputVariable1->setRange(0.000, 1.000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::Centroid(200));
outputVariable1->output()->setAccumulation(new fl::Maximum);

outputVariable1->addTerm(new fl::Triangle("LOW", 0.000, 0.250, 0.500));
outputVariable1->addTerm(new fl::Triangle("MEDIUM", 0.250, 0.500, 0.750));
outputVariable1->addTerm(new fl::Triangle("HIGH", 0.500, 0.750, 1.000));
engine->addOutputVariable(outputVariable1);

fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
ruleblock1->setName("");
ruleblock1->setTnorm(new fl::Minimum);
ruleblock1->setSnorm(new fl::Maximum);
ruleblock1->setActivation(new fl::Minimum);

ruleblock1->addRule(fl::FuzzyRule::parse("if Ambient is DARK then Power is HIGH", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
ruleblock1->addRule(fl::FuzzyRule::parse("if Ambient is BRIGHT then Power is LOW", engine));
engine->addRuleBlock(ruleblock1);
engine->addRuleBlock(ruleblock1);


}
