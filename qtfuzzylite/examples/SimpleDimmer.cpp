/*
 * SimpleDimmer.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include <fl/Headers.h>

using namespace fl;

int main(int argc, char* argv[]) {
    fl::Engine* engine = new fl::Engine;
    engine->setName("simple-dimmer");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("AmbientLight");
    inputVariable1->setRange(0.000, 1.000);

    inputVariable1->addTerm(new fl::Triangle("LOW", 0.000, 0.250, 0.500));
    inputVariable1->addTerm(new fl::Triangle("MEDIUM", 0.250, 0.500, 0.750));
    inputVariable1->addTerm(new fl::Triangle("HIGH", 0.500, 0.750, 1.000));
    engine->addInputVariable(inputVariable1);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("BulbPower");
    outputVariable1->setRange(0.000, 2.000);
    outputVariable1->setDefaultValue(std::numeric_limits<fl::scalar>::quiet_NaN());
    outputVariable1->setLockDefuzzifiedValue(true);
    outputVariable1->setDefuzzifier(new fl::Centroid(500));
    outputVariable1->output()->setAccumulation(new fl::Maximum);

    outputVariable1->addTerm(new fl::Triangle("LOW", 0.000, 0.500, 1.000));
    outputVariable1->addTerm(new fl::Triangle("MEDIUM", 0.500, 1.000, 1.500));
    outputVariable1->addTerm(new fl::Triangle("HIGH", 1.000, 1.500, 2.000));
    engine->addOutputVariable(outputVariable1);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::Minimum);
    ruleblock1->setSnorm(new fl::Maximum);
    ruleblock1->setActivation(new fl::Minimum);

    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if AmbientLight is LOW then BulbPower is LOW", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if AmbientLight is MEDIUM then BulbPower is MEDIUM", engine));
    ruleblock1->addRule(fl::MamdaniRule::parse(
            " if AmbientLight is HIGH then BulbPower is HIGH", engine));
    engine->addRuleBlock(ruleblock1);

//    engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid", FL_DEFAULT_DIVISIONS);

    scalar step = 1.0 / 10.0;
    InputVariable* ambientLight = engine->getInputVariable("AmbientLight");
    OutputVariable* bulbPower = engine->getOutputVariable("BulbPower");
    for (scalar input = ambientLight->getMinimum();
            input <= ambientLight->getMaximum() + step; input += step) {
        ambientLight->setInput(input);
        engine->process();
        FL_LOG("input=" << input << " -> output=" << bulbPower->defuzzify());
    }
}

