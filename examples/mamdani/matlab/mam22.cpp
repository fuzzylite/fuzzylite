#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("mam22");
engine->setDescription("");

InputVariable* angle = new InputVariable;
angle->setName("angle");
angle->setDescription("");
angle->setEnabled(true);
angle->setRange(-5.000, 5.000);
angle->setLockValueInRange(false);
angle->addTerm(new Bell("small", -5.000, 5.000, 8.000));
angle->addTerm(new Bell("big", 5.000, 5.000, 8.000));
engine->addInputVariable(angle);

InputVariable* velocity = new InputVariable;
velocity->setName("velocity");
velocity->setDescription("");
velocity->setEnabled(true);
velocity->setRange(-5.000, 5.000);
velocity->setLockValueInRange(false);
velocity->addTerm(new Bell("small", -5.000, 5.000, 2.000));
velocity->addTerm(new Bell("big", 5.000, 5.000, 2.000));
engine->addInputVariable(velocity);

OutputVariable* force = new OutputVariable;
force->setName("force");
force->setDescription("");
force->setEnabled(true);
force->setRange(-5.000, 5.000);
force->setLockValueInRange(false);
force->setAggregation(new Maximum);
force->setDefuzzifier(new Centroid(200));
force->setDefaultValue(fl::nan);
force->setLockPreviousValue(false);
force->addTerm(new Bell("negBig", -5.000, 1.670, 8.000));
force->addTerm(new Bell("negSmall", -1.670, 1.670, 8.000));
force->addTerm(new Bell("posSmall", 1.670, 1.670, 8.000));
force->addTerm(new Bell("posBig", 5.000, 1.670, 8.000));
engine->addOutputVariable(force);

OutputVariable* force2 = new OutputVariable;
force2->setName("force2");
force2->setDescription("");
force2->setEnabled(true);
force2->setRange(-5.000, 5.000);
force2->setLockValueInRange(false);
force2->setAggregation(new Maximum);
force2->setDefuzzifier(new Centroid(200));
force2->setDefaultValue(fl::nan);
force2->setLockPreviousValue(false);
force2->addTerm(new Bell("negBig2", -3.000, 1.670, 8.000));
force2->addTerm(new Bell("negSmall2", -1.000, 1.670, 8.000));
force2->addTerm(new Bell("posSmall2", 1.000, 1.670, 8.000));
force2->addTerm(new Bell("posBig2", 3.000, 1.670, 8.000));
engine->addOutputVariable(force2);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setImplication(new Minimum);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if angle is small and velocity is small then force is negBig and force2 is posBig2", engine));
ruleBlock->addRule(Rule::parse("if angle is small and velocity is big then force is negSmall and force2 is posSmall2", engine));
ruleBlock->addRule(Rule::parse("if angle is big and velocity is small then force is posSmall and force2 is negSmall2", engine));
ruleBlock->addRule(Rule::parse("if angle is big and velocity is big then force is posBig and force2 is negBig2", engine));
engine->addRuleBlock(ruleBlock);


}
