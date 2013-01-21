/* 
 * File:   mam22.cpp
 * Author: jcrada
 *
 * Created on 12 January 2013, 11:26 PM
 */

#include <fl/Headers.h>

int main(int argc, char* argv[]) {

    fl::Engine* engine = new fl::Engine;
    engine->setName("mam21");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("angle");
    inputVariable1->setRange(-5.000, 5.000);

    inputVariable1->addTerm(new fl::Bell("small", -5.000, 5.000, 8.000));
    inputVariable1->addTerm(new fl::Bell("big", 5.000, 5.000, 8.000));
    engine->addInputVariable(inputVariable1);

    fl::InputVariable* inputVariable2 = new fl::InputVariable;
    inputVariable2->setName("velocity");
    inputVariable2->setRange(-5.000, 5.000);

    inputVariable2->addTerm(new fl::Bell("small", -5.000, 5.000, 2.000));
    inputVariable2->addTerm(new fl::Bell("big", 5.000, 5.000, 2.000));
    engine->addInputVariable(inputVariable2);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("force");
    outputVariable1->setRange(-5.000, 5.000);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::Maximum);

    outputVariable1->addTerm(new fl::Bell("negBig", -5.000, 1.670, 8.000));
    outputVariable1->addTerm(new fl::Bell("negSmall", -1.670, 1.670, 8.000));
    outputVariable1->addTerm(new fl::Bell("posSmall", 1.670, 1.670, 8.000));
    outputVariable1->addTerm(new fl::Bell("posBig", 5.000, 1.670, 8.000));
    engine->addOutputVariable(outputVariable1);

    fl::OutputVariable* outputVariable2 = new fl::OutputVariable;
    outputVariable2->setName("force2");
    outputVariable2->setRange(-5.000, 5.000);
    outputVariable2->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable2->setLockDefuzzifiedValue(true);
    outputVariable2->setDefuzzifier(new fl::Centroid(500));
    outputVariable2->output()->setAccumulation(new fl::Maximum);

    outputVariable2->addTerm(new fl::Bell("negBig2", -3.000, 1.670, 8.000));
    outputVariable2->addTerm(new fl::Bell("negSmall2", -1.000, 1.670, 8.000));
    outputVariable2->addTerm(new fl::Bell("posSmall2", 1.000, 1.670, 8.000));
    outputVariable2->addTerm(new fl::Bell("posBig2", 3.000, 1.670, 8.000));
    engine->addOutputVariable(outputVariable2);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::Minimum);
    ruleblock1->setSnorm(new fl::Maximum);
    ruleblock1->setActivation(new fl::Minimum);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if angle is small and velocity is small then force is negBig and force2 is posBig2", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if angle is small and velocity is big then force is negSmall and force2 is posSmall2", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if angle is big and velocity is small then force is posSmall and force2 is negSmall2", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            "if angle is big and velocity is big then force is posBig and force2 is negBig2", engine));
    engine->addRuleBlock(ruleblock1);



}