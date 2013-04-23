/* 
 * File:   tank.cpp
 * Author: jcrada
 *
 * Created on 12 January 2013, 11:26 PM
 */

#include <fl/Headers.h>

int main(int argc, char* argv[]) {
    fl::Engine* engine = new fl::Engine;
    engine->setName("tank");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("level");
    inputVariable1->setRange(-1.000, 1.000);

    inputVariable1->addTerm(new fl::Gaussian("high", -1.000, 0.300));
    inputVariable1->addTerm(new fl::Gaussian("okay", 0.000, 0.300));
    inputVariable1->addTerm(new fl::Gaussian("low", 1.000, 0.300));
    engine->addInputVariable(inputVariable1);

    fl::InputVariable* inputVariable2 = new fl::InputVariable;
    inputVariable2->setName("rate");
    inputVariable2->setRange(-0.100, 0.100);

    inputVariable2->addTerm(new fl::Gaussian("negative", -0.100, 0.030));
    inputVariable2->addTerm(new fl::Gaussian("none", 0.000, 0.030));
    inputVariable2->addTerm(new fl::Gaussian("positive", 0.100, 0.030));
    engine->addInputVariable(inputVariable2);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("valve");
    outputVariable1->setRange(-1.000, 1.000);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::Maximum);

    outputVariable1->addTerm(new fl::Triangle("close_fast", -1.000, -0.900, -0.800));
    outputVariable1->addTerm(new fl::Triangle("close_slow", -0.600, -0.500, -0.400));
    outputVariable1->addTerm(new fl::Triangle("no_change", -0.100, 0.000, 0.100));
    outputVariable1->addTerm(new fl::Triangle("open_slow", 0.200, 0.300, 0.400));
    outputVariable1->addTerm(new fl::Triangle("open_fast", 0.800, 0.900, 1.000));
    engine->addOutputVariable(outputVariable1);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::AlgebraicProduct);
    ruleblock1->setSnorm(new fl::AlgebraicSum);
    ruleblock1->setActivation(new fl::AlgebraicProduct);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is okay then valve is no_change", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is low then valve is open_fast", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is high then valve is close_fast", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is okay and rate is positive then valve is close_slow", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is okay and rate is negative then valve is open_slow", engine));
    engine->addRuleBlock(ruleblock1);

}
