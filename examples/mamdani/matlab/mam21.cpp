#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("mam21");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("angle");
inputVariable1->setRange(-5.000, 5.000);
inputVariable1->addTerm(new Bell("small", -5.000, 5.000, 8.000));
inputVariable1->addTerm(new Bell("big", 5.000, 5.000, 8.000));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("velocity");
inputVariable2->setRange(-5.000, 5.000);
inputVariable2->addTerm(new Bell("small", -5.000, 5.000, 2.000));
inputVariable2->addTerm(new Bell("big", 5.000, 5.000, 2.000));
engine->addInputVariable(inputVariable2);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("force");
outputVariable->setRange(-5.000, 5.000);
outputVariable->fuzzyOutput()->setAccumulation(new Maximum);
outputVariable->setDefuzzifier(new Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(new Bell("negBig", -5.000, 1.670, 8.000));
outputVariable->addTerm(new Bell("negSmall", -1.670, 1.670, 8.000));
outputVariable->addTerm(new Bell("posSmall", 1.670, 1.670, 8.000));
outputVariable->addTerm(new Bell("posBig", 5.000, 1.670, 8.000));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setActivation(new Minimum);
ruleBlock->addRule(fl::Rule::parse("if angle is small and velocity is small then force is negBig", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is small and velocity is big then force is negSmall", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is big and velocity is small then force is posSmall", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is big and velocity is big then force is posBig", engine));
engine->addRuleBlock(ruleBlock);


}
