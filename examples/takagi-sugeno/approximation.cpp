#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("approximation");
engine->setDescription("");

InputVariable* inputX = new InputVariable;
inputX->setName("inputX");
inputX->setDescription("");
inputX->setEnabled(true);
inputX->setRange(0.000, 10.000);
inputX->setLockValueInRange(false);
inputX->addTerm(new Triangle("NEAR_1", 0.000, 1.000, 2.000));
inputX->addTerm(new Triangle("NEAR_2", 1.000, 2.000, 3.000));
inputX->addTerm(new Triangle("NEAR_3", 2.000, 3.000, 4.000));
inputX->addTerm(new Triangle("NEAR_4", 3.000, 4.000, 5.000));
inputX->addTerm(new Triangle("NEAR_5", 4.000, 5.000, 6.000));
inputX->addTerm(new Triangle("NEAR_6", 5.000, 6.000, 7.000));
inputX->addTerm(new Triangle("NEAR_7", 6.000, 7.000, 8.000));
inputX->addTerm(new Triangle("NEAR_8", 7.000, 8.000, 9.000));
inputX->addTerm(new Triangle("NEAR_9", 8.000, 9.000, 10.000));
engine->addInputVariable(inputX);

OutputVariable* outputFx = new OutputVariable;
outputFx->setName("outputFx");
outputFx->setDescription("");
outputFx->setEnabled(true);
outputFx->setRange(-1.000, 1.000);
outputFx->setLockValueInRange(false);
outputFx->setAggregation(fl::null);
outputFx->setDefuzzifier(new WeightedAverage("TakagiSugeno"));
outputFx->setDefaultValue(fl::nan);
outputFx->setLockPreviousValue(true);
outputFx->addTerm(new Constant("f1", 0.840));
outputFx->addTerm(new Constant("f2", 0.450));
outputFx->addTerm(new Constant("f3", 0.040));
outputFx->addTerm(new Constant("f4", -0.180));
outputFx->addTerm(new Constant("f5", -0.190));
outputFx->addTerm(new Constant("f6", -0.040));
outputFx->addTerm(new Constant("f7", 0.090));
outputFx->addTerm(new Constant("f8", 0.120));
outputFx->addTerm(new Constant("f9", 0.040));
engine->addOutputVariable(outputFx);

OutputVariable* trueFx = new OutputVariable;
trueFx->setName("trueFx");
trueFx->setDescription("");
trueFx->setEnabled(true);
trueFx->setRange(-1.000, 1.000);
trueFx->setLockValueInRange(false);
trueFx->setAggregation(fl::null);
trueFx->setDefuzzifier(new WeightedAverage("Automatic"));
trueFx->setDefaultValue(fl::nan);
trueFx->setLockPreviousValue(true);
trueFx->addTerm(Function::create("fx", "sin(inputX)/inputX", engine));
engine->addOutputVariable(trueFx);

OutputVariable* diffFx = new OutputVariable;
diffFx->setName("diffFx");
diffFx->setDescription("");
diffFx->setEnabled(true);
diffFx->setRange(-1.000, 1.000);
diffFx->setLockValueInRange(false);
diffFx->setAggregation(fl::null);
diffFx->setDefuzzifier(new WeightedAverage("Automatic"));
diffFx->setDefaultValue(fl::nan);
diffFx->setLockPreviousValue(false);
diffFx->addTerm(Function::create("diff", "fabs(outputFx-trueFx)", engine));
engine->addOutputVariable(diffFx);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(fl::null);
ruleBlock->setDisjunction(fl::null);
ruleBlock->setImplication(fl::null);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if inputX is NEAR_1 then outputFx is f1", engine));
ruleBlock->addRule(Rule::parse("if inputX is NEAR_2 then outputFx is f2", engine));
ruleBlock->addRule(Rule::parse("if inputX is NEAR_3 then outputFx is f3", engine));
ruleBlock->addRule(Rule::parse("if inputX is NEAR_4 then outputFx is f4", engine));
ruleBlock->addRule(Rule::parse("if inputX is NEAR_5 then outputFx is f5", engine));
ruleBlock->addRule(Rule::parse("if inputX is NEAR_6 then outputFx is f6", engine));
ruleBlock->addRule(Rule::parse("if inputX is NEAR_7 then outputFx is f7", engine));
ruleBlock->addRule(Rule::parse("if inputX is NEAR_8 then outputFx is f8", engine));
ruleBlock->addRule(Rule::parse("if inputX is NEAR_9 then outputFx is f9", engine));
ruleBlock->addRule(Rule::parse("if inputX is any  then trueFx is fx and diffFx is diff", engine));
engine->addRuleBlock(ruleBlock);


}
