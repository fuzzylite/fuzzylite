#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("mam21");

fl::InputVariable* inputVariable1 = new fl::InputVariable;
inputVariable1->setEnabled(true);
inputVariable1->setName("angle");
inputVariable1->setRange(-5.000, 5.000);
inputVariable1->addTerm(new fl::Bell("small", -5.000, 5.000, 8.000));
inputVariable1->addTerm(new fl::Bell("big", 5.000, 5.000, 8.000));
engine->addInputVariable(inputVariable1);

fl::InputVariable* inputVariable2 = new fl::InputVariable;
inputVariable2->setEnabled(true);
inputVariable2->setName("velocity");
inputVariable2->setRange(-5.000, 5.000);
inputVariable2->addTerm(new fl::Bell("small", -5.000, 5.000, 2.000));
inputVariable2->addTerm(new fl::Bell("big", 5.000, 5.000, 2.000));
engine->addInputVariable(inputVariable2);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setEnabled(true);
outputVariable1->setName("force");
outputVariable1->setRange(-5.000, 5.000);
outputVariable1->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable1->setDefuzzifier(new fl::Centroid(200));
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setLockOutputRange(false);
outputVariable1->addTerm(new fl::Bell("negBig", -5.000, 1.670, 8.000));
outputVariable1->addTerm(new fl::Bell("negSmall", -1.670, 1.670, 8.000));
outputVariable1->addTerm(new fl::Bell("posSmall", 1.670, 1.670, 8.000));
outputVariable1->addTerm(new fl::Bell("posBig", 5.000, 1.670, 8.000));
engine->addOutputVariable(outputVariable1);

fl::OutputVariable* outputVariable2 = new fl::OutputVariable;
outputVariable2->setEnabled(true);
outputVariable2->setName("force2");
outputVariable2->setRange(-5.000, 5.000);
outputVariable2->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable2->setDefuzzifier(new fl::Centroid(200));
outputVariable2->setDefaultValue(fl::nan);
outputVariable2->setLockValidOutput(false);
outputVariable2->setLockOutputRange(false);
outputVariable2->addTerm(new fl::Bell("negBig2", -3.000, 1.670, 8.000));
outputVariable2->addTerm(new fl::Bell("negSmall2", -1.000, 1.670, 8.000));
outputVariable2->addTerm(new fl::Bell("posSmall2", 1.000, 1.670, 8.000));
outputVariable2->addTerm(new fl::Bell("posBig2", 3.000, 1.670, 8.000));
engine->addOutputVariable(outputVariable2);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if angle is small and velocity is small then force is negBig and force2 is posBig2", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is small and velocity is big then force is negSmall and force2 is posSmall2", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is big and velocity is small then force is posSmall and force2 is negSmall2", engine));
ruleBlock->addRule(fl::Rule::parse("if angle is big and velocity is big then force is posBig and force2 is negBig2", engine));
engine->addRuleBlock(ruleBlock);


}
