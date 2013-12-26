#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("");

fl::InputVariable* inputVariable = new fl::InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("X");
inputVariable->setRange(-10.00000000, 10.00000000);
inputVariable->addTerm(new fl::Bell("small", -10.00000000, 5.00000000, 3.00000000));
inputVariable->addTerm(new fl::Bell("medium", 0.00000000, 5.00000000, 3.00000000));
inputVariable->addTerm(new fl::Bell("large", 10.00000000, 5.00000000, 3.00000000));
engine->addInputVariable(inputVariable);

fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
outputVariable1->setEnabled(true);
outputVariable1->setName("A");
outputVariable1->setRange(0.00000000, 1.00000000);
outputVariable1->setLockOutputRange(false);
outputVariable1->setDefaultValue(fl::nan);
outputVariable1->setLockValidOutput(false);
outputVariable1->setDefuzzifier(new fl::WeightedAverage);
outputVariable1->fuzzyOutput()->setAccumulation(NULL);
outputVariable1->addTerm(new fl::Ramp("a1", 0.00000000, 0.25000000));
outputVariable1->addTerm(new fl::Ramp("a2", 0.60000000, 0.40000000));
outputVariable1->addTerm(new fl::Ramp("a3", 0.70000000, 1.00000000));
engine->addOutputVariable(outputVariable1);

fl::OutputVariable* outputVariable2 = new fl::OutputVariable;
outputVariable2->setEnabled(true);
outputVariable2->setName("B");
outputVariable2->setRange(0.00000000, 1.00000000);
outputVariable2->setLockOutputRange(false);
outputVariable2->setDefaultValue(fl::nan);
outputVariable2->setLockValidOutput(false);
outputVariable2->setDefuzzifier(new fl::WeightedAverage);
outputVariable2->fuzzyOutput()->setAccumulation(NULL);
outputVariable2->addTerm(new fl::Sigmoid("b1", 0.13000000, 30.00000000));
outputVariable2->addTerm(new fl::Sigmoid("b2", 0.50000000, -30.00000000));
outputVariable2->addTerm(new fl::Sigmoid("b3", 0.83000000, 30.00000000));
engine->addOutputVariable(outputVariable2);

fl::OutputVariable* outputVariable3 = new fl::OutputVariable;
outputVariable3->setEnabled(true);
outputVariable3->setName("C");
outputVariable3->setRange(0.00000000, 1.00000000);
outputVariable3->setLockOutputRange(false);
outputVariable3->setDefaultValue(fl::nan);
outputVariable3->setLockValidOutput(false);
outputVariable3->setDefuzzifier(new fl::WeightedAverage);
outputVariable3->fuzzyOutput()->setAccumulation(NULL);
outputVariable3->addTerm(new fl::SShape("c1", 0.00000000, 0.25000000));
outputVariable3->addTerm(new fl::ZShape("c2", 0.30000000, 0.60000000));
outputVariable3->addTerm(new fl::SShape("c3", 0.70000000, 1.00000000));
engine->addOutputVariable(outputVariable3);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(NULL);
ruleBlock->setDisjunction(NULL);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if X is small then A is a1 and B is b1 and C is c1", engine));
ruleBlock->addRule(fl::Rule::parse("if X is medium then A is a2 and B is b2 and C is c2", engine));
ruleBlock->addRule(fl::Rule::parse("if X is large then A is a3 and B is b3 and C is c3", engine));
engine->addRuleBlock(ruleBlock);


}
