/*
 * Example2.cpp
 *
 *  Created on: 17/12/2012
 *      Author: jcrada
 */

#include "fl/example/Example2.h"

namespace fl {

    Example2::Example2() : Example() {
        create();
    }

    void Example2::create() {
        engine = new Engine("simple-dimmer");

        InputVariable* ambientLight = new InputVariable("AmbientLight");
        ambientLight->addTerm(new Triangle("LOW", .0, .25, .5));
        ambientLight->addTerm(new Triangle("MEDIUM", .25, .5, .75));
        ambientLight->addTerm(new Triangle("HIGH", .5, .75, 1));
        engine->addInputVariable(ambientLight);

        InputVariable* electricity = new InputVariable("Electricity");
        electricity->addTerm(new Sigmoid("CHEAP", .5, -7));
        electricity->addTerm(new Gaussian("REGULAR", 0.7, .2));
        electricity->addTerm(new Sigmoid("EXPENSIVE", 1, 7));
        engine->addInputVariable(electricity);

        OutputVariable* bulbPower = new OutputVariable("BulbPower",
                std::numeric_limits<scalar>::quiet_NaN());
        bulbPower->addTerm(new Triangle("LOW", 0.0, 0.5, 1));
        bulbPower->addTerm(new Triangle("LOW_MEDIUM", 0.5, 1, 1.5));
        bulbPower->addTerm(new Triangle("MEDIUM", 1, 1.5, 2));
        bulbPower->addTerm(new Triangle("MEDIUM_HIGH", 1.5, 2, 2.5));
        bulbPower->addTerm(new Triangle("HIGH", 2, 2.5, 3));
        engine->addOutputVariable(bulbPower);

        RuleBlock* ruleblock = new RuleBlock();
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is LOW and Electricity is CHEAP then BulbPower is MEDIUM               ", engine));
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is LOW and Electricity is REGULAR then BulbPower is MEDIUM_HIGH        ", engine));
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is LOW and Electricity is EXPENSIVE then BulbPower is LOW              ", engine));
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is MEDIUM and Electricity is CHEAP then BulbPower is MEDIUM_HIGH       ", engine));
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is MEDIUM and Electricity is REGULAR then BulbPower is MEDIUM          ", engine));
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is MEDIUM and Electricity is EXPENSIVE then BulbPower is LOW_MEDIUM    ", engine));
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is HIGH and Electricity is CHEAP then BulbPower is HIGH                ", engine));
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is HIGH and Electricity is REGULAR then BulbPower is LOW_MEDIUM        ", engine));
        ruleblock->addRule(MamdaniRule::parse("if AmbientLight is HIGH and Electricity is EXPENSIVE then BulbPower is MEDIUM          ", engine));

        engine->addRuleBlock(ruleblock);

        engine->configure(new Configuration);
    }

    void Example2::test() {
        FclExporter exporter;
        FL_LOG("Make sure fuzzylite was compiled with -DFL_DEBUG=true to display the demo");
        FL_LOG("Fuzzy Control Language:" << std::endl << exporter.toFcl(engine));
        scalar step = 1.0 / 10.0;
        InputVariable* ambientLight = engine->getInputVariable("AmbientLight");
        OutputVariable* bulbPower = engine->getOutputVariable("BulbPower");
        for (scalar input = ambientLight->getMinimum();
                input <= ambientLight->getMaximum() + step; input += step) {
            ambientLight->setInput(input);
            engine->process();
            bulbPower->defuzzify();
        }
    }

    void Example2::main() {
        fl::Example2 sm;
        FL_LOG("Example: " << sm.name());
        FL_LOG("========");
        sm.create();
        sm.test();
    }

} 
