#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("sltbu");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("distance");
inputVariable1->setRange(0.000, 25.000);
inputVariable1->addTerm(new fl::ZShape("near", 1.000, 2.000));
inputVariable1->addTerm(new fl::SShape("far", 1.000, 2.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("control1");
inputVariable2->setRange(-0.785, 0.785);
engine->addInputVariable(inputVariable2);

fl::InputVariable* inputVariable3 = new fl::InputVariable;
inputVariable3->setEnabled(true);
inputVariable3->setName("control2");
inputVariable3->setRange(-0.785, 0.785);
engine->addInputVariable(inputVariable3);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("control");
outputVariable->setRange(-0.785, 0.785);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::WeightedAverage);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(fl::Linear::create("out1mf1", engine->inputVariables(), 0.000, 0.000, 1.000, 0.000));
outputVariable->addTerm(fl::Linear::create("out1mf2", engine->inputVariables(), 0.000, 1.000, 0.000, 0.000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::AlgebraicProduct);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::AlgebraicProduct);
ruleBlock->addRule(fl::Rule::parse("if distance is near then control is out1mf1", engine));
ruleBlock->addRule(fl::Rule::parse("if distance is far then control is out1mf2", engine));
engine->addRuleBlock(ruleBlock);


}
