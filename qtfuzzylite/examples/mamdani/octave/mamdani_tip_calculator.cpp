/* 
 * File:   mamdani_tip_calculator.cpp
 * Author: jcrada
 *
 * Created on 12 January 2013, 11:03 PM
 */
#include <fl/Headers.h>

int main(int argc, char* argv[]) {
    fl::Engine* engine = new fl::Engine;
    engine->setName("Mamdani-Tip-Calculator");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("Food-Quality");
    inputVariable1->setRange(1.000, 10.000);

    inputVariable1->addTerm(new fl::Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
    inputVariable1->addTerm(new fl::Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
    engine->addInputVariable(inputVariable1);

    fl::InputVariable* inputVariable2 = new fl::InputVariable;
    inputVariable2->setName("Service");
    inputVariable2->setRange(1.000, 10.000);

    inputVariable2->addTerm(new fl::Trapezoid("Bad", 0.000, 1.000, 3.000, 7.000));
    inputVariable2->addTerm(new fl::Trapezoid("Good", 3.000, 7.000, 10.000, 11.000));
    engine->addInputVariable(inputVariable2);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("Tip");
    outputVariable1->setRange(0.000, 30.000);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::AlgebraicSum);

    outputVariable1->addTerm(new fl::Gaussian("About-Ten-Percent", 10.000, 2.000));
    outputVariable1->addTerm(new fl::Gaussian("About-Fifteen-Percent", 15.000, 2.000));
    outputVariable1->addTerm(new fl::Gaussian("About-Twenty-Percent", 20.000, 2.000));
    engine->addOutputVariable(outputVariable1);

    fl::OutputVariable* outputVariable2 = new fl::OutputVariable;
    outputVariable2->setName("Check-Plus-Tip");
    outputVariable2->setRange(1.000, 1.300);
    outputVariable2->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable2->setLockDefuzzifiedValue(true);
    outputVariable2->setDefuzzifier(new fl::Centroid(500));
    outputVariable2->output()->setAccumulation(new fl::AlgebraicSum);

    outputVariable2->addTerm(new fl::Gaussian("Plus-About-Ten-Percent", 1.100, 0.020));
    outputVariable2->addTerm(new fl::Gaussian("Plus-About-Fifteen-Percent", 1.150, 0.020));
    outputVariable2->addTerm(new fl::Gaussian("Plus-About-Twenty-Percent", 1.200, 0.020));
    engine->addOutputVariable(outputVariable2);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::AlgebraicProduct);
    ruleblock1->setSnorm(new fl::Maximum);
    ruleblock1->setActivation(new fl::Minimum);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if Food-Quality is Bad and Service is Bad  then Tip is About-Ten-Percent and Check-Plus-Tip is Plus-About-Ten-Percent", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if Food-Quality is Bad and Service is Good  then Tip is About-Fifteen-Percent and Check-Plus-Tip is Plus-About-Fifteen-Percent", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if Food-Quality is Good and Service is Bad  then Tip is About-Fifteen-Percent and Check-Plus-Tip is Plus-About-Fifteen-Percent", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if Food-Quality is Good and Service is Good  then Tip is About-Twenty-Percent and Check-Plus-Tip is Plus-About-Twenty-Percent", engine));
    engine->addRuleBlock(ruleblock1);


}
