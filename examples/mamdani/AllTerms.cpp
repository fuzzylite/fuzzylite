#include <fl/Headers.h>

int main(int argc, char** argv){
fl::Engine* engine = new fl::Engine;
engine->setName("qtfuzzylite");

fl::InputVariable* inputVariable = new fl::InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("AllInputTerms");
inputVariable->setRange(0.000, 6.500);
inputVariable->addTerm(new fl::Sigmoid("A", 0.500, -20.000));
inputVariable->addTerm(new fl::ZShape("B", 0.000, 1.000));
inputVariable->addTerm(new fl::Ramp("C", 1.000, 0.000));
inputVariable->addTerm(new fl::Triangle("D", 0.500, 1.000, 1.500));
inputVariable->addTerm(new fl::Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
inputVariable->addTerm(new fl::Rectangle("F", 1.750, 2.250));
inputVariable->addTerm(fl::Discrete::create("G", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
inputVariable->addTerm(new fl::Gaussian("H", 3.000, 0.200));
inputVariable->addTerm(new fl::GaussianProduct("I", 3.500, 0.100, 3.300, 0.300));
inputVariable->addTerm(new fl::Bell("J", 4.000, 0.250, 3.000));
inputVariable->addTerm(new fl::PiShape("K", 4.000, 4.500, 4.500, 5.000));
inputVariable->addTerm(new fl::SigmoidDifference("L", 4.750, 10.000, 30.000, 5.250));
inputVariable->addTerm(new fl::SigmoidProduct("M", 5.250, 20.000, -10.000, 5.750));
inputVariable->addTerm(new fl::Ramp("N", 5.500, 6.500));
inputVariable->addTerm(new fl::SShape("O", 5.500, 6.500));
inputVariable->addTerm(new fl::Sigmoid("P", 6.000, 20.000));
engine->addInputVariable(inputVariable);

fl::OutputVariable* outputVariable = new fl::OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("AllOutputTerms");
outputVariable->setRange(0.000, 6.500);
outputVariable->fuzzyOutput()->setAccumulation(new fl::Maximum);
outputVariable->setDefuzzifier(new fl::Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockValidOutput(false);
outputVariable->setLockOutputRange(false);
outputVariable->addTerm(new fl::Sigmoid("A", 0.500, -20.000));
outputVariable->addTerm(new fl::ZShape("B", 0.000, 1.000));
outputVariable->addTerm(new fl::Ramp("C", 1.000, 0.000));
outputVariable->addTerm(new fl::Triangle("D", 0.500, 1.000, 1.500));
outputVariable->addTerm(new fl::Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
outputVariable->addTerm(new fl::Rectangle("F", 1.750, 2.250));
outputVariable->addTerm(fl::Discrete::create("G", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
outputVariable->addTerm(new fl::Gaussian("H", 3.000, 0.200));
outputVariable->addTerm(new fl::GaussianProduct("I", 3.500, 0.100, 3.300, 0.300));
outputVariable->addTerm(new fl::Bell("J", 4.000, 0.250, 3.000));
outputVariable->addTerm(new fl::PiShape("K", 4.000, 4.500, 4.500, 5.000));
outputVariable->addTerm(new fl::SigmoidDifference("L", 4.750, 10.000, 30.000, 5.250));
outputVariable->addTerm(new fl::SigmoidProduct("M", 5.250, 20.000, -10.000, 5.750));
outputVariable->addTerm(new fl::Ramp("N", 5.500, 6.500));
outputVariable->addTerm(new fl::SShape("O", 5.500, 6.500));
outputVariable->addTerm(new fl::Sigmoid("P", 6.000, 20.000));
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
