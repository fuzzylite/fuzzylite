#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("mam21");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("angle");
inputVariable1->setRange(-5.00000000, 5.00000000);
inputVariable1->addTerm(new fl::Bell("small", -5.00000000, 5.00000000, 8.00000000));
inputVariable1->addTerm(new fl::Bell("big", 5.00000000, 5.00000000, 8.00000000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("velocity");
inputVariable2->setRange(-5.00000000, 5.00000000);
inputVariable2->addTerm(new fl::Bell("small", -5.00000000, 5.00000000, 2.00000000));
inputVariable2->addTerm(new fl::Bell("big", 5.00000000, 5.00000000, 2.00000000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("force");
outputVariable->setRange(-5.00000000, 5.00000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(new fl::Bell("negBig", -5.00000000, 1.67000000, 8.00000000));
outputVariable->addTerm(new fl::Bell("negSmall", -1.67000000, 1.67000000, 8.00000000));
outputVariable->addTerm(new fl::Bell("posSmall", 1.67000000, 1.67000000, 8.00000000));
outputVariable->addTerm(new fl::Bell("posBig", 5.00000000, 1.67000000, 8.00000000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if angle is small and velocity is small then force is negBig", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is small and velocity is big then force is negSmall", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is big and velocity is small then force is posSmall", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is big and velocity is big then force is posBig", engine));
engine->addRuleBlock(ruleBlock);


}
