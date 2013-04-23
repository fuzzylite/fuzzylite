
#include <fl/Headers.h>

int main(int argc, char* argv[]) {
    fl::Engine* engine = new fl::Engine;
    engine->setName("qtfuzzylite");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("AllInputTerms");
    inputVariable1->setRange(0.000, 6.500);

    inputVariable1->addTerm(new fl::Sigmoid("A", 0.500, -20.000));
    inputVariable1->addTerm(new fl::ZShape("B", 0.000, 1.000));
    inputVariable1->addTerm(new fl::Ramp("C", 1.000, 0.000));
    inputVariable1->addTerm(new fl::Triangle("D", 0.500, 1.000, 1.500));
    inputVariable1->addTerm(new fl::Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
    inputVariable1->addTerm(new fl::Rectangle("F", 1.750, 2.250));
    inputVariable1->addTerm(new fl::Discrete("G", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
    inputVariable1->addTerm(new fl::Gaussian("H", 3.000, 0.200));
    inputVariable1->addTerm(new fl::GaussianProduct("I", 3.500, 0.100, 3.300, 0.300));
    inputVariable1->addTerm(new fl::Bell("J", 4.000, 0.250, 3.000));
    inputVariable1->addTerm(new fl::PiShape("K", 4.000, 4.500, 4.500, 5.000));
    inputVariable1->addTerm(new fl::SigmoidDifference("L", 4.750, 10.000, 30.000, 5.250));
    inputVariable1->addTerm(new fl::SigmoidProduct("M", 5.250, 20.000, -10.000, 5.750));
    inputVariable1->addTerm(new fl::Ramp("N", 5.500, 6.500));
    inputVariable1->addTerm(new fl::SShape("O", 5.500, 6.500));
    inputVariable1->addTerm(new fl::Sigmoid("P", 6.000, 20.000));
    engine->addInputVariable(inputVariable1);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("AllOutputTerms");
    outputVariable1->setRange(0.000, 6.500);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::Maximum);

    outputVariable1->addTerm(new fl::Sigmoid("A", 0.500, -20.000));
    outputVariable1->addTerm(new fl::ZShape("B", 0.000, 1.000));
    outputVariable1->addTerm(new fl::Ramp("C", 1.000, 0.000));
    outputVariable1->addTerm(new fl::Triangle("D", 0.500, 1.000, 1.500));
    outputVariable1->addTerm(new fl::Trapezoid("E", 1.000, 1.250, 1.750, 2.000));
    outputVariable1->addTerm(new fl::Rectangle("F", 1.750, 2.250));
    outputVariable1->addTerm(new fl::Discrete("G", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
    outputVariable1->addTerm(new fl::Gaussian("H", 3.000, 0.200));
    outputVariable1->addTerm(new fl::GaussianProduct("I", 3.500, 0.100, 3.300, 0.300));
    outputVariable1->addTerm(new fl::Bell("J", 4.000, 0.250, 3.000));
    outputVariable1->addTerm(new fl::PiShape("K", 4.000, 4.500, 4.500, 5.000));
    outputVariable1->addTerm(new fl::SigmoidDifference("L", 4.750, 10.000, 30.000, 5.250));
    outputVariable1->addTerm(new fl::SigmoidProduct("M", 5.250, 20.000, -10.000, 5.750));
    outputVariable1->addTerm(new fl::Ramp("N", 5.500, 6.500));
    outputVariable1->addTerm(new fl::SShape("O", 5.500, 6.500));
    outputVariable1->addTerm(new fl::Sigmoid("P", 6.000, 20.000));
    engine->addOutputVariable(outputVariable1);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::Minimum);
    ruleblock1->setSnorm(new fl::Maximum);
    ruleblock1->setActivation(new fl::Minimum);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is A then AllOutputTerms is P", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is B then AllOutputTerms is O", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is C then AllOutputTerms is N", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is D then AllOutputTerms is M", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is E then AllOutputTerms is L", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is F then AllOutputTerms is K", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is G then AllOutputTerms is J", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is H then AllOutputTerms is I", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is I then AllOutputTerms is H", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is J then AllOutputTerms is G", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is K then AllOutputTerms is F", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is L then AllOutputTerms is E", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is M then AllOutputTerms is D", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is N then AllOutputTerms is C", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is O then AllOutputTerms is B", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if AllInputTerms is P then AllOutputTerms is A", engine));
    engine->addRuleBlock(ruleblock1);

}

