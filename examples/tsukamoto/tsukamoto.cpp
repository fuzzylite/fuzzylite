#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("");

InputVariable* X = new InputVariable;
X->setEnabled(true);
X->setName("X");
X->setRange(-10.000, 10.000);
X->setLockValueInRange(false);
X->addTerm(new Bell("small", -10.000, 5.000, 3.000));
X->addTerm(new Bell("medium", 0.000, 5.000, 3.000));
X->addTerm(new Bell("large", 10.000, 5.000, 3.000));
engine->addInputVariable(X);

OutputVariable* Ramps = new OutputVariable;
Ramps->setEnabled(true);
Ramps->setName("Ramps");
Ramps->setRange(0.000, 1.000);
Ramps->setLockValueInRange(false);
Ramps->fuzzyOutput()->setAccumulation(fl::null);
Ramps->setDefuzzifier(new WeightedAverage("Automatic"));
Ramps->setDefaultValue(fl::nan);
Ramps->setLockPreviousValue(false);
Ramps->addTerm(new Ramp("b", 0.600, 0.400));
Ramps->addTerm(new Ramp("a", 0.000, 0.250));
Ramps->addTerm(new Ramp("c", 0.700, 1.000));
engine->addOutputVariable(Ramps);

OutputVariable* Sigmoids = new OutputVariable;
Sigmoids->setEnabled(true);
Sigmoids->setName("Sigmoids");
Sigmoids->setRange(0.020, 1.000);
Sigmoids->setLockValueInRange(false);
Sigmoids->fuzzyOutput()->setAccumulation(fl::null);
Sigmoids->setDefuzzifier(new WeightedAverage("Automatic"));
Sigmoids->setDefaultValue(fl::nan);
Sigmoids->setLockPreviousValue(false);
Sigmoids->addTerm(new Sigmoid("b", 0.500, -30.000));
Sigmoids->addTerm(new Sigmoid("a", 0.130, 30.000));
Sigmoids->addTerm(new Sigmoid("c", 0.830, 30.000));
engine->addOutputVariable(Sigmoids);

OutputVariable* ZSShapes = new OutputVariable;
ZSShapes->setEnabled(true);
ZSShapes->setName("ZSShapes");
ZSShapes->setRange(0.000, 1.000);
ZSShapes->setLockValueInRange(false);
ZSShapes->fuzzyOutput()->setAccumulation(fl::null);
ZSShapes->setDefuzzifier(new WeightedAverage("Automatic"));
ZSShapes->setDefaultValue(fl::nan);
ZSShapes->setLockPreviousValue(false);
ZSShapes->addTerm(new ZShape("b", 0.300, 0.600));
ZSShapes->addTerm(new SShape("a", 0.000, 0.250));
ZSShapes->addTerm(new SShape("c", 0.700, 1.000));
engine->addOutputVariable(ZSShapes);

OutputVariable* Concaves = new OutputVariable;
Concaves->setEnabled(true);
Concaves->setName("Concaves");
Concaves->setRange(0.000, 1.000);
Concaves->setLockValueInRange(false);
Concaves->fuzzyOutput()->setAccumulation(fl::null);
Concaves->setDefuzzifier(new WeightedAverage("Automatic"));
Concaves->setDefaultValue(fl::nan);
Concaves->setLockPreviousValue(false);
Concaves->addTerm(new Concave("b", 0.500, 0.400));
Concaves->addTerm(new Concave("a", 0.240, 0.250));
Concaves->addTerm(new Concave("c", 0.900, 1.000));
engine->addOutputVariable(Concaves);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->addRule(fl::Rule::parse("if X is small then Ramps is a and Sigmoids is a and ZSShapes is a and Concaves is a", engine));
ruleBlock->addRule(fl::Rule::parse("if X is medium then Ramps is b and Sigmoids is b and ZSShapes is b and Concaves is b", engine));
ruleBlock->addRule(fl::Rule::parse("if X is large then Ramps is c and Sigmoids is c and ZSShapes is c and Concaves is c", engine));
engine->addRuleBlock(ruleBlock);


}
