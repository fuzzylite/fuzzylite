#include <fl/Headers.h>

int main(int argc, char** argv){
using namespace fl;

Engine* engine = new Engine;
engine->setName("qtfuzzylite");

InputVariable* inputVariable = new InputVariable;
inputVariable->setEnabled(true);
inputVariable->setName("AllInputTerms");
inputVariable->setRange(0.000, 6.500);
inputVariable->addTerm(new Sigmoid("A", 0.500, -20.000));
inputVariable->addTerm(new ZShape("B", 0.000, 1.000));
inputVariable->addTerm(new Ramp("C", 1.000, 0.000));
inputVariable->addTerm(new Triangle("D", 0.500, 1.000, 1.500));
inputVariable->addTerm(new Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
inputVariable->addTerm(new Concave("F", 0.850, 0.250));
inputVariable->addTerm(new Rectangle("G", 1.750, 2.250));
inputVariable->addTerm(Discrete::create("H", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
inputVariable->addTerm(new Gaussian("I", 3.000, 0.200));
inputVariable->addTerm(new Cosine("J", 3.250, 0.650));
inputVariable->addTerm(new GaussianProduct("K", 3.500, 0.100, 3.300, 0.300));
inputVariable->addTerm(new Spike("L", 3.640, 1.040));
inputVariable->addTerm(new Bell("M", 4.000, 0.250, 3.000));
inputVariable->addTerm(new PiShape("N", 4.000, 4.500, 4.500, 5.000));
inputVariable->addTerm(new Concave("O", 5.650, 6.250));
inputVariable->addTerm(new SigmoidDifference("P", 4.750, 10.000, 30.000, 5.250));
inputVariable->addTerm(new SigmoidProduct("Q", 5.250, 20.000, -10.000, 5.750));
inputVariable->addTerm(new Ramp("R", 5.500, 6.500));
inputVariable->addTerm(new SShape("S", 5.500, 6.500));
inputVariable->addTerm(new Sigmoid("T", 6.000, 20.000));
engine->addInputVariable(inputVariable);

OutputVariable* outputVariable = new OutputVariable;
outputVariable->setEnabled(true);
outputVariable->setName("AllOutputTerms");
outputVariable->setRange(0.000, 6.500);
outputVariable->fuzzyOutput()->setAccumulation(new Maximum);
outputVariable->setDefuzzifier(new Centroid(200));
outputVariable->setDefaultValue(fl::nan);
outputVariable->setLockPreviousOutputValue(false);
outputVariable->setLockOutputValueInRange(false);
outputVariable->addTerm(new Sigmoid("A", 0.500, -20.000));
outputVariable->addTerm(new ZShape("B", 0.000, 1.000));
outputVariable->addTerm(new Ramp("C", 1.000, 0.000));
outputVariable->addTerm(new Triangle("D", 0.500, 1.000, 1.500));
outputVariable->addTerm(new Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
outputVariable->addTerm(new Concave("F", 0.850, 0.250));
outputVariable->addTerm(new Rectangle("G", 1.750, 2.250));
outputVariable->addTerm(Discrete::create("H", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
outputVariable->addTerm(new Gaussian("I", 3.000, 0.200));
outputVariable->addTerm(new Cosine("J", 3.250, 0.650));
outputVariable->addTerm(new GaussianProduct("K", 3.500, 0.100, 3.300, 0.300));
outputVariable->addTerm(new Spike("L", 3.640, 1.040));
outputVariable->addTerm(new Bell("M", 4.000, 0.250, 3.000));
outputVariable->addTerm(new PiShape("N", 4.000, 4.500, 4.500, 5.000));
outputVariable->addTerm(new Concave("O", 5.650, 6.250));
outputVariable->addTerm(new SigmoidDifference("P", 4.750, 10.000, 30.000, 5.250));
outputVariable->addTerm(new SigmoidProduct("Q", 5.250, 20.000, -10.000, 5.750));
outputVariable->addTerm(new Ramp("R", 5.500, 6.500));
outputVariable->addTerm(new SShape("S", 5.500, 6.500));
outputVariable->addTerm(new Sigmoid("T", 6.000, 20.000));
engine->addOutputVariable(outputVariable);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setEnabled(true);
ruleBlock->setName("");
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setActivation(new Minimum);
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is A then AllOutputTerms is T", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is B then AllOutputTerms is S", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is C then AllOutputTerms is R", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is D then AllOutputTerms is Q", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is E then AllOutputTerms is P", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is F then AllOutputTerms is O", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is G then AllOutputTerms is N", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is H then AllOutputTerms is M", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is I then AllOutputTerms is L", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is J then AllOutputTerms is K", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is K then AllOutputTerms is J", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is L then AllOutputTerms is I", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is M then AllOutputTerms is H", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is N then AllOutputTerms is G", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is O then AllOutputTerms is F", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is P then AllOutputTerms is E", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is Q then AllOutputTerms is D", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is R then AllOutputTerms is C", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is S then AllOutputTerms is B", engine));
ruleBlock->addRule(fl::Rule::parse("if AllInputTerms is T then AllOutputTerms is A", engine));
engine->addRuleBlock(ruleBlock);


}
