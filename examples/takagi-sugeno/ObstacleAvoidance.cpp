#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("ObstacleAvoidance");
engine->setDescription("");

InputVariable* obstacle = new InputVariable;
obstacle->setName("obstacle");
obstacle->setDescription("");
obstacle->setEnabled(true);
obstacle->setRange(0.000, 1.000);
obstacle->setLockValueInRange(false);
obstacle->addTerm(new Ramp("left", 1.000, 0.000));
obstacle->addTerm(new Ramp("right", 0.000, 1.000));
engine->addInputVariable(obstacle);

OutputVariable* tsSteer = new OutputVariable;
tsSteer->setName("tsSteer");
tsSteer->setDescription("");
tsSteer->setEnabled(true);
tsSteer->setRange(0.000, 1.000);
tsSteer->setLockValueInRange(false);
tsSteer->setAggregation(new Maximum);
tsSteer->setDefuzzifier(new WeightedAverage("Automatic"));
tsSteer->setDefaultValue(fl::nan);
tsSteer->setLockPreviousValue(false);
tsSteer->addTerm(new Constant("left", 0.333));
tsSteer->addTerm(new Constant("right", 0.666));
engine->addOutputVariable(tsSteer);

RuleBlock* takagiSugeno = new RuleBlock;
takagiSugeno->setName("takagiSugeno");
takagiSugeno->setDescription("");
takagiSugeno->setEnabled(true);
takagiSugeno->setConjunction(fl::null);
takagiSugeno->setDisjunction(fl::null);
takagiSugeno->setImplication(fl::null);
takagiSugeno->setActivation(new General);
takagiSugeno->addRule(Rule::parse("if obstacle is left then tsSteer is right", engine));
takagiSugeno->addRule(Rule::parse("if obstacle is right then tsSteer is left", engine));
engine->addRuleBlock(takagiSugeno);


}
