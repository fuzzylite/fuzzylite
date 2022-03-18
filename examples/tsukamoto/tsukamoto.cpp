#include <fuzzylite/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fuzzylite;

Engine* engine = new Engine;
engine->setName("tsukamoto");
engine->setDescription("");

InputVariable* X = new InputVariable;
X->setName("X");
X->setDescription("");
X->setEnabled(true);
X->setRange(-10.000, 10.000);
X->setLockValueInRange(false);
X->addTerm(new Bell("small", -10.000, 5.000, 3.000));
X->addTerm(new Bell("medium", 0.000, 5.000, 3.000));
X->addTerm(new Bell("large", 10.000, 5.000, 3.000));
engine->addInputVariable(X);

OutputVariable* Ramps = new OutputVariable;
Ramps->setName("Ramps");
Ramps->setDescription("");
Ramps->setEnabled(true);
Ramps->setRange(0.000, 1.000);
Ramps->setLockValueInRange(false);
Ramps->setAggregation(fl::null);
Ramps->setDefuzzifier(new WeightedAverage("Automatic"));
Ramps->setDefaultValue(fl::nan);
Ramps->setLockPreviousValue(false);
Ramps->addTerm(new Ramp("b", 0.600, 0.400));
Ramps->addTerm(new Ramp("a", 0.000, 0.250));
Ramps->addTerm(new Ramp("c", 0.700, 1.000));
engine->addOutputVariable(Ramps);

OutputVariable* Sigmoids = new OutputVariable;
Sigmoids->setName("Sigmoids");
Sigmoids->setDescription("");
Sigmoids->setEnabled(true);
Sigmoids->setRange(0.020, 1.000);
Sigmoids->setLockValueInRange(false);
Sigmoids->setAggregation(fl::null);
Sigmoids->setDefuzzifier(new WeightedAverage("Automatic"));
Sigmoids->setDefaultValue(fl::nan);
Sigmoids->setLockPreviousValue(false);
Sigmoids->addTerm(new Sigmoid("b", 0.500, -30.000));
Sigmoids->addTerm(new Sigmoid("a", 0.130, 30.000));
Sigmoids->addTerm(new Sigmoid("c", 0.830, 30.000));
engine->addOutputVariable(Sigmoids);

OutputVariable* ZSShapes = new OutputVariable;
ZSShapes->setName("ZSShapes");
ZSShapes->setDescription("");
ZSShapes->setEnabled(true);
ZSShapes->setRange(0.000, 1.000);
ZSShapes->setLockValueInRange(false);
ZSShapes->setAggregation(fl::null);
ZSShapes->setDefuzzifier(new WeightedAverage("Automatic"));
ZSShapes->setDefaultValue(fl::nan);
ZSShapes->setLockPreviousValue(false);
ZSShapes->addTerm(new ZShape("b", 0.300, 0.600));
ZSShapes->addTerm(new SShape("a", 0.000, 0.250));
ZSShapes->addTerm(new SShape("c", 0.700, 1.000));
engine->addOutputVariable(ZSShapes);

OutputVariable* Concaves = new OutputVariable;
Concaves->setName("Concaves");
Concaves->setDescription("");
Concaves->setEnabled(true);
Concaves->setRange(0.000, 1.000);
Concaves->setLockValueInRange(false);
Concaves->setAggregation(fl::null);
Concaves->setDefuzzifier(new WeightedAverage("Automatic"));
Concaves->setDefaultValue(fl::nan);
Concaves->setLockPreviousValue(false);
Concaves->addTerm(new Concave("b", 0.500, 0.400));
Concaves->addTerm(new Concave("a", 0.240, 0.250));
Concaves->addTerm(new Concave("c", 0.900, 1.000));
engine->addOutputVariable(Concaves);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if X is small then Ramps is a and Sigmoids is a and ZSShapes is a and Concaves is a", engine));
ruleBlock->addRule(Rule::parse("if X is medium then Ramps is b and Sigmoids is b and ZSShapes is b and Concaves is b", engine));
ruleBlock->addRule(Rule::parse("if X is large then Ramps is c and Sigmoids is c and ZSShapes is c and Concaves is c", engine));
engine->addRuleBlock(ruleBlock);


}
