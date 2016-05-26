#include <fl/Headers.h>

int main(int argc, char** argv){
//C++ code generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("mam21");

InputVariable* angle = new InputVariable;
angle->setEnabled(true);
angle->setName("angle");
angle->setRange(-5.000, 5.000);
angle->setLockValueInRange(false);
angle->addTerm(new Bell("small", -5.000, 5.000, 8.000));
angle->addTerm(new Bell("big", 5.000, 5.000, 8.000));
engine->addInputVariable(angle);

InputVariable* velocity = new InputVariable;
velocity->setEnabled(true);
velocity->setName("velocity");
velocity->setRange(-5.000, 5.000);
velocity->setLockValueInRange(false);
velocity->addTerm(new Bell("small", -5.000, 5.000, 2.000));
velocity->addTerm(new Bell("big", 5.000, 5.000, 2.000));
engine->addInputVariable(velocity);

OutputVariable* force = new OutputVariable;
force->setEnabled(true);
force->setName("force");
force->setRange(-5.000, 5.000);
force->setLockValueInRange(false);
force->fuzzyOutput()->setAggregation(new Maximum);
force->setDefuzzifier(new Centroid(200));
force->setDefaultValue(fl::nan);
force->setLockPreviousValue(false);
force->addTerm(new Bell("negBig", -5.000, 1.670, 8.000));
force->addTerm(new Bell("negSmall", -1.670, 1.670, 8.000));
force->addTerm(new Bell("posSmall", 1.670, 1.670, 8.000));
force->addTerm(new Bell("posBig", 5.000, 1.670, 8.000));
engine->addOutputVariable(force);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setImplication(new Minimum);
ruleBlock->setActivation(new General);
ruleBlock->addRule(fl::Rule::parse("if angle is small and velocity is small then force is negBig", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is small and velocity is big then force is negSmall", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is big and velocity is small then force is posSmall", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is big and velocity is big then force is posBig", engine));
engine->addRuleBlock(ruleBlock);


}
