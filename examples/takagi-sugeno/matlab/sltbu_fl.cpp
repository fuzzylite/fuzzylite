#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("sltbu");

InputVariable* inputVariable1 = new InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("distance");
inputVariable1->setRange(0.000, 25.000);
inputVariable1->addTerm(new ZShape("near", 1.000, 2.000));
inputVariable1->addTerm(new SShape("far", 1.000, 2.000));
engine->addInputVariable(inputVariable1);

InputVariable* inputVariable2 = new InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("control1");
inputVariable2->setRange(-0.785, 0.785);
engine->addInputVariable(inputVariable2);

InputVariable* inputVariable3 = new InputVariable;
inputVariable3->setEnabled(true);
inputVariable3->setName("control2");
inputVariable3->setRange(-0.785, 0.785);
engine->addInputVariable(inputVariable3);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("control");
outputVariable->setRange(-0.785, 0.785);
outputVariable->fuzzyOutput()->setAccumulation(fl::null);
outputVariable->setDefuzzifier(new WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(Linear::create("out1mf1", engine, 0.000, 0.000, 1.000, 0.000));
outputVariable->addTerm(Linear::create("out1mf2", engine, 0.000, 1.000, 0.000, 0.000));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setActivation(fl::null);
ruleBlock->addRule(fl::Rule::parse("if distance is near then control is out1mf1", engine));
ruleBlock->addRule(fl::Rule::parse("if distance is far then control is out1mf2", engine));
engine->addRuleBlock(ruleBlock);


}
