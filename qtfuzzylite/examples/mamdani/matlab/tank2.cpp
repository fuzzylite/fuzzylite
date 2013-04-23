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

    inputVariable1->addTerm(new fl::Trapezoid("high", -2.000, -1.000, -0.800, -0.001));
    inputVariable1->addTerm(new fl::Triangle("good", -0.150, 0.000, 0.500));
    inputVariable1->addTerm(new fl::Trapezoid("low", 0.001, 0.800, 1.000, 1.500));
    engine->addInputVariable(inputVariable1);

    fl::InputVariable* inputVariable2 = new fl::InputVariable;
    inputVariable2->setName("change");
    inputVariable2->setRange(-0.100, 0.100);

    inputVariable2->addTerm(new fl::Trapezoid("falling", -0.140, -0.100, -0.060, 0.000));
    inputVariable2->addTerm(new fl::Trapezoid("rising", -0.001, 0.060, 0.100, 0.140));
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
    outputVariable1->addTerm(new fl::Triangle("open_slow", 0.400, 0.500, 0.600));
    outputVariable1->addTerm(new fl::Triangle("open_fast", 0.800, 0.900, 1.000));
    engine->addOutputVariable(outputVariable1);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::AlgebraicProduct);
    ruleblock1->setSnorm(new fl::AlgebraicSum);
    ruleblock1->setActivation(new fl::AlgebraicProduct);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is low then valve is open_fast", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is high then valve is close_fast", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is good and change is rising then valve is close_slow", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if level is good and change is falling then valve is open_slow", engine));
    engine->addRuleBlock(ruleblock1);


}
