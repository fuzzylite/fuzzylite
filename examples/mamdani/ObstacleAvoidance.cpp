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

OutputVariable* mSteer = new OutputVariable;
mSteer->setName("mSteer");
mSteer->setDescription("");
mSteer->setEnabled(true);
mSteer->setRange(0.000, 1.000);
mSteer->setLockValueInRange(false);
mSteer->setAggregation(new Maximum);
mSteer->setDefuzzifier(new Centroid(100));
mSteer->setDefaultValue(fl::nan);
mSteer->setLockPreviousValue(false);
mSteer->addTerm(new Ramp("left", 1.000, 0.000));
mSteer->addTerm(new Ramp("right", 0.000, 1.000));
engine->addOutputVariable(mSteer);

RuleBlock* mamdani = new RuleBlock;
mamdani->setName("mamdani");
mamdani->setDescription("");
mamdani->setEnabled(true);
mamdani->setConjunction(fl::null);
mamdani->setDisjunction(fl::null);
mamdani->setImplication(new AlgebraicProduct);
mamdani->setActivation(new General);
mamdani->addRule(Rule::parse("if obstacle is left then mSteer is right", engine));
mamdani->addRule(Rule::parse("if obstacle is right then mSteer is left", engine));
engine->addRuleBlock(mamdani);


}
