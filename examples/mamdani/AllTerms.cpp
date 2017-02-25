#include <fl/Headers.h>

int main(int argc, char** argv){
//Code automatically generated with fuzzylite 6.0.

using namespace fl;

Engine* engine = new Engine;
engine->setName("AllTerms");
engine->setDescription("");

InputVariable* AllInputTerms = new InputVariable;
AllInputTerms->setName("AllInputTerms");
AllInputTerms->setDescription("");
AllInputTerms->setEnabled(true);
AllInputTerms->setRange(0.000, 6.500);
AllInputTerms->setLockValueInRange(false);
AllInputTerms->addTerm(new Sigmoid("A", 0.500, -20.000));
AllInputTerms->addTerm(new ZShape("B", 0.000, 1.000));
AllInputTerms->addTerm(new Ramp("C", 1.000, 0.000));
AllInputTerms->addTerm(new Triangle("D", 0.500, 1.000, 1.500));
AllInputTerms->addTerm(new Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
AllInputTerms->addTerm(new Concave("F", 0.850, 0.250));
AllInputTerms->addTerm(new Rectangle("G", 1.750, 2.250));
AllInputTerms->addTerm(Discrete::create("H", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
AllInputTerms->addTerm(new Gaussian("I", 3.000, 0.200));
AllInputTerms->addTerm(new Cosine("J", 3.250, 0.650));
AllInputTerms->addTerm(new GaussianProduct("K", 3.500, 0.100, 3.300, 0.300));
AllInputTerms->addTerm(new Spike("L", 3.640, 1.040));
AllInputTerms->addTerm(new Bell("M", 4.000, 0.250, 3.000));
AllInputTerms->addTerm(new PiShape("N", 4.000, 4.500, 4.500, 5.000));
AllInputTerms->addTerm(new Concave("O", 5.650, 6.250));
AllInputTerms->addTerm(new SigmoidDifference("P", 4.750, 10.000, 30.000, 5.250));
AllInputTerms->addTerm(new SigmoidProduct("Q", 5.250, 20.000, -10.000, 5.750));
AllInputTerms->addTerm(new Ramp("R", 5.500, 6.500));
AllInputTerms->addTerm(new SShape("S", 5.500, 6.500));
AllInputTerms->addTerm(new Sigmoid("T", 6.000, 20.000));
engine->addInputVariable(AllInputTerms);

OutputVariable* AllOutputTerms = new OutputVariable;
AllOutputTerms->setName("AllOutputTerms");
AllOutputTerms->setDescription("");
AllOutputTerms->setEnabled(true);
AllOutputTerms->setRange(0.000, 6.500);
AllOutputTerms->setLockValueInRange(false);
AllOutputTerms->setAggregation(new Maximum);
AllOutputTerms->setDefuzzifier(new Centroid(200));
AllOutputTerms->setDefaultValue(fl::nan);
AllOutputTerms->setLockPreviousValue(false);
AllOutputTerms->addTerm(new Sigmoid("A", 0.500, -20.000));
AllOutputTerms->addTerm(new ZShape("B", 0.000, 1.000));
AllOutputTerms->addTerm(new Ramp("C", 1.000, 0.000));
AllOutputTerms->addTerm(new Triangle("D", 0.500, 1.000, 1.500));
AllOutputTerms->addTerm(new Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
AllOutputTerms->addTerm(new Concave("F", 0.850, 0.250));
AllOutputTerms->addTerm(new Rectangle("G", 1.750, 2.250));
AllOutputTerms->addTerm(Discrete::create("H", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
AllOutputTerms->addTerm(new Gaussian("I", 3.000, 0.200));
AllOutputTerms->addTerm(new Cosine("J", 3.250, 0.650));
AllOutputTerms->addTerm(new GaussianProduct("K", 3.500, 0.100, 3.300, 0.300));
AllOutputTerms->addTerm(new Spike("L", 3.640, 1.040));
AllOutputTerms->addTerm(new Bell("M", 4.000, 0.250, 3.000));
AllOutputTerms->addTerm(new PiShape("N", 4.000, 4.500, 4.500, 5.000));
AllOutputTerms->addTerm(new Concave("O", 5.650, 6.250));
AllOutputTerms->addTerm(new SigmoidDifference("P", 4.750, 10.000, 30.000, 5.250));
AllOutputTerms->addTerm(new SigmoidProduct("Q", 5.250, 20.000, -10.000, 5.750));
AllOutputTerms->addTerm(new Ramp("R", 5.500, 6.500));
AllOutputTerms->addTerm(new SShape("S", 5.500, 6.500));
AllOutputTerms->addTerm(new Sigmoid("T", 6.000, 20.000));
engine->addOutputVariable(AllOutputTerms);

RuleBlock* ruleBlock = new RuleBlock;
ruleBlock->setName("");
ruleBlock->setDescription("");
ruleBlock->setEnabled(true);
ruleBlock->setConjunction(new Minimum);
ruleBlock->setDisjunction(new Maximum);
ruleBlock->setImplication(new Minimum);
ruleBlock->setActivation(new General);
ruleBlock->addRule(Rule::parse("if AllInputTerms is A then AllOutputTerms is T", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is B then AllOutputTerms is S", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is C then AllOutputTerms is R", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is D then AllOutputTerms is Q", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is E then AllOutputTerms is P", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is F then AllOutputTerms is O", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is G then AllOutputTerms is N", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is H then AllOutputTerms is M", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is I then AllOutputTerms is L", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is J then AllOutputTerms is K", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is K then AllOutputTerms is J", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is L then AllOutputTerms is I", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is M then AllOutputTerms is H", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is N then AllOutputTerms is G", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is O then AllOutputTerms is F", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is P then AllOutputTerms is E", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is Q then AllOutputTerms is D", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is R then AllOutputTerms is C", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is S then AllOutputTerms is B", engine));
ruleBlock->addRule(Rule::parse("if AllInputTerms is T then AllOutputTerms is A", engine));
engine->addRuleBlock(ruleBlock);


}
