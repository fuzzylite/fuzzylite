
#include <fl/Headers.h>

int main(int argc, char* argv[]) {
    fl::Engine* engine = new fl::Engine;
    engine->setName("all-terms");

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("InputAllTerms");
    inputVariable1->setMinimum(-2.000);
    inputVariable1->setMaximum(17.400);
    inputVariable1->addTerm(new fl::Sigmoid("A", -0.060, -2.462));
    inputVariable1->addTerm(new fl::ZShape("B", -5.492, 4.984));
    inputVariable1->addTerm(new fl::Triangle("C", 0.000, 0.500, 1.000));
    inputVariable1->addTerm(new fl::Trapezoid("D", 0.240, 0.780, 1.500, 2.000));
    inputVariable1->addTerm(new fl::Rectangle("E", 1.050, 2.190));
    inputVariable1->addTerm(new fl::Discrete("F", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
    inputVariable1->addTerm(new fl::Gaussian("G", 3.500, 0.550));
    inputVariable1->addTerm(new fl::GaussianProduct("H", 4.750, 0.926, 3.910, 0.998));
    inputVariable1->addTerm(new fl::Bell("I", 5.928, 0.988, 3.000));
    inputVariable1->addTerm(new fl::PiShape("J", 4.598, 8.060, 7.354, 9.214));
    inputVariable1->addTerm(new fl::SigmoidDifference("K", 8.424, 5.306, 10.584, 1.526));
    inputVariable1->addTerm(new fl::SigmoidProduct("L", 9.648, 1.390, 12.672, -9.598));
    inputVariable1->addTerm(new fl::Ramp("M", 8.208, 14.688));
    inputVariable1->addTerm(new fl::SShape("N", 8.670, 18.952));
    engine->addInputVariable(inputVariable1);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("OutputAllTerms");
    outputVariable1->setMinimum(-2.000);
    outputVariable1->setMaximum(17.400);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::Maximum);
    outputVariable1->addTerm(new fl::Sigmoid("A", -0.060, -2.462));
    outputVariable1->addTerm(new fl::ZShape("B", -5.492, 4.984));
    outputVariable1->addTerm(new fl::Triangle("C", 0.000, 0.500, 1.000));
    outputVariable1->addTerm(new fl::Trapezoid("D", 0.240, 0.780, 1.500, 2.000));
    outputVariable1->addTerm(new fl::Rectangle("E", 1.050, 2.190));
    outputVariable1->addTerm(new fl::Discrete("F", 10, 2.000, 0.000, 2.250, 1.000, 2.500, 0.500, 2.750, 1.000, 3.000, 0.000));
    outputVariable1->addTerm(new fl::Gaussian("G", 3.500, 0.550));
    outputVariable1->addTerm(new fl::GaussianProduct("H", 4.750, 0.926, 3.910, 0.998));
    outputVariable1->addTerm(new fl::Bell("I", 5.928, 0.988, 3.000));
    outputVariable1->addTerm(new fl::PiShape("J", 4.598, 8.060, 7.354, 9.214));
    outputVariable1->addTerm(new fl::SigmoidDifference("K", 8.424, 5.306, 10.584, 1.526));
    outputVariable1->addTerm(new fl::SigmoidProduct("L", 9.648, 1.390, 12.672, -9.598));
    outputVariable1->addTerm(new fl::Ramp("M", 8.208, 14.688));
    outputVariable1->addTerm(new fl::SShape("N", 8.670, 18.952));
    engine->addOutputVariable(outputVariable1);


    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::Minimum);
    ruleblock1->setSnorm(new fl::Maximum);
    ruleblock1->setActivation(new fl::Minimum);
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is A then OutputAllTerms is N", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is B then OutputAllTerms is M", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is C then OutputAllTerms is L", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is D then OutputAllTerms is K", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is E then OutputAllTerms is J", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is F then OutputAllTerms is I", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is G then OutputAllTerms is H", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is H then OutputAllTerms is G", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is I then OutputAllTerms is F", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is J then OutputAllTerms is E", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is K then OutputAllTerms is D", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is L then OutputAllTerms is C", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is M then OutputAllTerms is B", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse("if InputAllTerms is N then OutputAllTerms is A", engine));
    engine->addRuleBlock(ruleblock1);

}

