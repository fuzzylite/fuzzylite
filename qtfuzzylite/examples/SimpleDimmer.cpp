/*
 * SimpleDimmer.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include <fl/Headers.h>

using namespace fl;

int main(int argc, char* argv[]) {
    Engine* engine = new Engine("simple-dimmer");

    InputVariable* ambientLight = new InputVariable("AmbientLight", 0, 1);
    ambientLight->addTerm(new Triangle("LOW", .0, .25, .5));
    ambientLight->addTerm(new Triangle("MEDIUM", .25, .5, .75));
    ambientLight->addTerm(new Triangle("HIGH", .5, .75, 1));
    engine->addInputVariable(ambientLight);


    OutputVariable* bulbPower = new OutputVariable("BulbPower", 0, 2);
    bulbPower->setDefaultValue(std::numeric_limits<scalar>::quiet_NaN());
    bulbPower->setLockDefuzzifiedValue(true);
    bulbPower->addTerm(new Triangle("LOW", 0.0, 0.5, 1));
    bulbPower->addTerm(new Triangle("MEDIUM", 0.5, 1, 1.5));
    bulbPower->addTerm(new Triangle("HIGH", 1, 1.5, 2));
    engine->addOutputVariable(bulbPower);

    RuleBlock* ruleblock = new RuleBlock();
    ruleblock->addRule(MamdaniRule::parse("if AmbientLight is LOW then BulbPower is LOW", engine));
    ruleblock->addRule(MamdaniRule::parse("if AmbientLight is MEDIUM then BulbPower is MEDIUM", engine));
    ruleblock->addRule(MamdaniRule::parse("if AmbientLight is HIGH then BulbPower is HIGH", engine));

    engine->addRuleBlock(ruleblock);
    
    engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid", FL_DEFAULT_DIVISIONS);

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

