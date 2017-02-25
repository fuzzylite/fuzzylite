#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("sltbu_fl");
engine->setDescription("");

InputVariable* distance = new InputVariable;
distance->setName("distance");
distance->setDescription("");
distance->setEnabled(true);
distance->setRange(0.000, 25.000);
distance->setLockValueInRange(false);
distance->addTerm(new ZShape("near", 1.000, 2.000));
distance->addTerm(new SShape("far", 1.000, 2.000));
engine->addInputVariable(distance);

InputVariable* control1 = new InputVariable;
control1->setName("control1");
control1->setDescription("");
control1->setEnabled(true);
control1->setRange(-0.785, 0.785);
control1->setLockValueInRange(false);
engine->addInputVariable(control1);

InputVariable* control2 = new InputVariable;
control2->setName("control2");
control2->setDescription("");
control2->setEnabled(true);
control2->setRange(-0.785, 0.785);
control2->setLockValueInRange(false);
engine->addInputVariable(control2);

OutputVariable* control = new OutputVariable;
control->setName("control");
control->setDescription("");
control->setEnabled(true);
control->setRange(-0.785, 0.785);
control->setLockValueInRange(false);
control->setAggregation(fl::null);
control->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
control->setDefaultValue(fl::nan);
control->setLockPreviousValue(false);
control->addTerm(Linear::create("out1mf1", engine, 0.000, 0.000, 1.000, 0.000));
control->addTerm(Linear::create("out1mf2", engine, 0.000, 1.000, 0.000, 0.000));
engine->addOutputVariable(control);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if distance is near then control is out1mf1", engine));
ruleBlock->addRule(Rule::parse("if distance is far then control is out1mf2", engine));
engine->addRuleBlock(ruleBlock);


}
