/* 
 * File:   tipper.cpp
 * Author: jcrada
 *
 * Created on 12 January 2013, 11:26 PM
 */

#include <fl/Headers.h>

int main(int argc, char* argv[]) {
    fl::Engine* engine = new fl::Engine;
    engine->setName("tipper");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("service");
    inputVariable1->setRange(0.000, 10.000);

    inputVariable1->addTerm(new fl::Gaussian("poor", 0.000, 1.500));
    inputVariable1->addTerm(new fl::Gaussian("good", 5.000, 1.500));
    inputVariable1->addTerm(new fl::Gaussian("excellent", 10.000, 1.500));
    engine->addInputVariable(inputVariable1);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("tip");
    outputVariable1->setRange(0.000, 30.000);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::Maximum);

    outputVariable1->addTerm(new fl::Triangle("cheap", 0.000, 5.000, 10.000));
    outputVariable1->addTerm(new fl::Triangle("average", 10.000, 15.000, 20.000));
    outputVariable1->addTerm(new fl::Triangle("generous", 20.000, 25.000, 30.000));
    engine->addOutputVariable(outputVariable1);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::Minimum);
    ruleblock1->setSnorm(new fl::Maximum);
    ruleblock1->setActivation(new fl::Minimum);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if service is poor then tip is cheap", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if service is good then tip is average", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if service is excellent then tip is generous", engine));
    engine->addRuleBlock(ruleblock1);

}
