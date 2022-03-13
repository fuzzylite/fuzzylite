#include <fuzzylite/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fuzzylite;

Engine* engine = new Engine;
engine->setName("SimpleDimmer");
engine->setDescription("");

InputVariable* Ambient = new InputVariable;
Ambient->setName("Ambient");
Ambient->setDescription("");
Ambient->setEnabled(true);
Ambient->setRange(0.000, 1.000);
Ambient->setLockValueInRange(false);
Ambient->addTerm(new Triangle("DARK", 0.000, 0.250, 0.500));
Ambient->addTerm(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
Ambient->addTerm(new Triangle("BRIGHT", 0.500, 0.750, 1.000));
engine->addInputVariable(Ambient);

OutputVariable* Power = new OutputVariable;
Power->setName("Power");
Power->setDescription("");
Power->setEnabled(true);
Power->setRange(0.000, 1.000);
Power->setLockValueInRange(false);
Power->setAggregation(fl::null);
Power->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
Power->setDefaultValue(fl::nan);
Power->setLockPreviousValue(false);
Power->addTerm(new Constant("LOW", 0.250));
Power->addTerm(new Constant("MEDIUM", 0.500));
Power->addTerm(new Constant("HIGH", 0.750));
engine->addOutputVariable(Power);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if Ambient is DARK then Power is HIGH", engine));
ruleBlock->addRule(Rule::parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
ruleBlock->addRule(Rule::parse("if Ambient is BRIGHT then Power is LOW", engine));
engine->addRuleBlock(ruleBlock);


}
