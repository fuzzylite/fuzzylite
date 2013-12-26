#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("qtfuzzylite");

fl::InputVariable* inputVariable = new fl::InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("AllInputTerms");
inputVariable->setRange(0.00000000, 6.50000000);
inputVariable->addTerm(new fl::Sigmoid("A", 0.50000000, -20.00000000));
inputVariable->addTerm(new fl::ZShape("B", 0.00000000, 1.00000000));
inputVariable->addTerm(new fl::Ramp("C", 1.00000000, 0.00000000));
inputVariable->addTerm(new fl::Triangle("D", 0.50000000, 1.00000000, 1.50000000));
inputVariable->addTerm(new fl::Trapezoid("E", 1.00000000, 1.25000000, 1.75000000, 2.00000000));
inputVariable->addTerm(new fl::Rectangle("F", 1.75000000, 2.25000000));
inputVariable->addTerm(fl::Discrete::create("G", 10, 2.00000000, 0.00000000, 2.25000000, 1.00000000, 2.50000000, 0.50000000, 2.75000000, 1.00000000, 3.00000000, 0.00000000));
inputVariable->addTerm(new fl::Gaussian("H", 3.00000000, 0.20000000));
inputVariable->addTerm(new fl::GaussianProduct("I", 3.50000000, 0.10000000, 3.30000000, 0.30000000));
inputVariable->addTerm(new fl::Bell("J", 4.00000000, 0.25000000, 3.00000000));
inputVariable->addTerm(new fl::PiShape("K", 4.00000000, 4.50000000, 4.50000000, 5.00000000));
inputVariable->addTerm(new fl::SigmoidDifference("L", 4.75000000, 10.00000000, 30.00000000, 5.25000000));
inputVariable->addTerm(new fl::SigmoidProduct("M", 5.25000000, 20.00000000, -10.00000000, 5.75000000));
inputVariable->addTerm(new fl::Ramp("N", 5.50000000, 6.50000000));
inputVariable->addTerm(new fl::SShape("O", 5.50000000, 6.50000000));
inputVariable->addTerm(new fl::Sigmoid("P", 6.00000000, 20.00000000));
engine->addInputVariable(inputVariable);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("AllOutputTerms");
outputVariable->setRange(0.00000000, 6.50000000);
outputVariable->setLockOutputRange(false);
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->addTerm(new fl::Sigmoid("A", 0.50000000, -20.00000000));
outputVariable->addTerm(new fl::ZShape("B", 0.00000000, 1.00000000));
outputVariable->addTerm(new fl::Ramp("C", 1.00000000, 0.00000000));
outputVariable->addTerm(new fl::Triangle("D", 0.50000000, 1.00000000, 1.50000000));
outputVariable->addTerm(new fl::Trapezoid("E", 1.00000000, 1.25000000, 1.75000000, 2.00000000));
outputVariable->addTerm(new fl::Rectangle("F", 1.75000000, 2.25000000));
outputVariable->addTerm(fl::Discrete::create("G", 10, 2.00000000, 0.00000000, 2.25000000, 1.00000000, 2.50000000, 0.50000000, 2.75000000, 1.00000000, 3.00000000, 0.00000000));
outputVariable->addTerm(new fl::Gaussian("H", 3.00000000, 0.20000000));
outputVariable->addTerm(new fl::GaussianProduct("I", 3.50000000, 0.10000000, 3.30000000, 0.30000000));
outputVariable->addTerm(new fl::Bell("J", 4.00000000, 0.25000000, 3.00000000));
outputVariable->addTerm(new fl::PiShape("K", 4.00000000, 4.50000000, 4.50000000, 5.00000000));
outputVariable->addTerm(new fl::SigmoidDifference("L", 4.75000000, 10.00000000, 30.00000000, 5.25000000));
outputVariable->addTerm(new fl::SigmoidProduct("M", 5.25000000, 20.00000000, -10.00000000, 5.75000000));
outputVariable->addTerm(new fl::Ramp("N", 5.50000000, 6.50000000));
outputVariable->addTerm(new fl::SShape("O", 5.50000000, 6.50000000));
outputVariable->addTerm(new fl::Sigmoid("P", 6.00000000, 20.00000000));
engine->addOutputVariable(outputVariable);

fl::RuleBlock* ruleBlock = new fl::RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new fl::Minimum);
ruleBlock->setDisjunction(new fl::Maximum);
ruleBlock->setActivation(new fl::Minimum);
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is A then AllOutputTerms is P", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is B then AllOutputTerms is O", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is C then AllOutputTerms is N", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is D then AllOutputTerms is M", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is E then AllOutputTerms is L", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is F then AllOutputTerms is K", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is G then AllOutputTerms is J", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is H then AllOutputTerms is I", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is I then AllOutputTerms is H", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is J then AllOutputTerms is G", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is K then AllOutputTerms is F", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is L then AllOutputTerms is E", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is M then AllOutputTerms is D", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is N then AllOutputTerms is C", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is O then AllOutputTerms is B", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is P then AllOutputTerms is A", engine));
engine->addRuleBlock(ruleBlock);


}
