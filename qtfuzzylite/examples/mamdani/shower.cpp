/* 
 * File:   shower.cpp
 * Author: jcrada
 *
 * Created on 12 January 2013, 11:26 PM
 */

#include <fl/Headers.h>

int main(int argc, char* argv[]) {
    fl::Engine* engine = new fl::Engine;
    engine->setName("shower");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("temp");
    inputVariable1->setRange(-20.000, 20.000);

    inputVariable1->addTerm(new fl::Trapezoid("cold", -30.000, -30.000, -15.000, 0.000));
    inputVariable1->addTerm(new fl::Triangle("good", -10.000, 0.000, 10.000));
    inputVariable1->addTerm(new fl::Trapezoid("hot", 0.000, 15.000, 30.000, 30.000));
    engine->addInputVariable(inputVariable1);

    fl::InputVariable* inputVariable2 = new fl::InputVariable;
    inputVariable2->setName("flow");
    inputVariable2->setRange(-1.000, 1.000);

    inputVariable2->addTerm(new fl::Trapezoid("soft", -3.000, -3.000, -0.800, 0.000));
    inputVariable2->addTerm(new fl::Triangle("good", -0.400, 0.000, 0.400));
    inputVariable2->addTerm(new fl::Trapezoid("hard", 0.000, 0.800, 3.000, 3.000));
    engine->addInputVariable(inputVariable2);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("cold");
    outputVariable1->setRange(-1.000, 1.000);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::Maximum);

    outputVariable1->addTerm(new fl::Triangle("closeFast", -1.000, -0.600, -0.300));
    outputVariable1->addTerm(new fl::Triangle("closeSlow", -0.600, -0.300, 0.000));
    outputVariable1->addTerm(new fl::Triangle("steady", -0.300, 0.000, 0.300));
    outputVariable1->addTerm(new fl::Triangle("openSlow", 0.000, 0.300, 0.600));
    outputVariable1->addTerm(new fl::Triangle("openFast", 0.300, 0.600, 1.000));
    engine->addOutputVariable(outputVariable1);

    fl::OutputVariable* outputVariable2 = new fl::OutputVariable;
    outputVariable2->setName("hot");
    outputVariable2->setRange(-1.000, 1.000);
    outputVariable2->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable2->setLockDefuzzifiedValue(true);
    outputVariable2->setDefuzzifier(new fl::Centroid(500));
    outputVariable2->output()->setAccumulation(new fl::Maximum);

    outputVariable2->addTerm(new fl::Triangle("closeFast", -1.000, -0.600, -0.300));
    outputVariable2->addTerm(new fl::Triangle("closeSlow", -0.600, -0.300, 0.000));
    outputVariable2->addTerm(new fl::Triangle("steady", -0.300, 0.000, 0.300));
    outputVariable2->addTerm(new fl::Triangle("openSlow", 0.000, 0.300, 0.600));
    outputVariable2->addTerm(new fl::Triangle("openFast", 0.300, 0.600, 1.000));
    engine->addOutputVariable(outputVariable2);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::Minimum);
    ruleblock1->setSnorm(new fl::Maximum);
    ruleblock1->setActivation(new fl::Minimum);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is cold and flow is soft then cold is openSlow and hot is openFast", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is cold and flow is good then cold is closeSlow and hot is openSlow", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is cold and flow is hard then cold is closeFast and hot is closeSlow", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is good and flow is soft then cold is openSlow and hot is openSlow", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is good and flow is good then cold is steady and hot is steady", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is good and flow is hard then cold is closeSlow and hot is closeSlow", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is hot and flow is soft then cold is openFast and hot is openSlow", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is hot and flow is good then cold is openSlow and hot is closeSlow", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if temp is hot and flow is hard then cold is closeSlow and hot is closeFast", engine));
    engine->addRuleBlock(ruleblock1);

}
