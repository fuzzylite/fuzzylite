/*
 * SimpleMamdani.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/example/Example1.h"

namespace fl {

    Example1::Example1() :
            Example() {
        create();
    }

    void Example1::create() {
        engine = new Engine("health-and-energy");

        InputVariable* energy = new InputVariable("Energy");
        energy->addTerm(new Triangle("LOW", .0, .25, .5));
        energy->addTerm(new Triangle("MEDIUM", .25, .5, .75));
        energy->addTerm(new Triangle("HIGH", .5, .75, 1));
        engine->addInputVariable(energy);

        OutputVariable* health = new OutputVariable("Health",
                std::numeric_limits<scalar>::quiet_NaN());
        health->addTerm(new Triangle("BAD", 0.0, 0.5, 1));
        health->addTerm(new Triangle("REGULAR", 0.5, 1, 1.5));
        health->addTerm(new Triangle("GOOD", 1, 1.5, 2));
        engine->addOutputVariable(health);

        RuleBlock* ruleblock = new RuleBlock();
        ruleblock->addRule(MamdaniRule::parse("if Energy is LOW then Health is BAD", engine));
        ruleblock->addRule(MamdaniRule::parse("if Energy is MEDIUM then Health is REGULAR", engine));
        ruleblock->addRule(MamdaniRule::parse("if Energy is HIGH then Health is GOOD", engine));

        engine->addRuleBlock(ruleblock);

        engine->configure(new Configuration);
    }

    void Example1::test() {
        FclExporter exporter;
        FL_LOG("Make sure fuzzylite was compiled with -DFL_DEBUG=true to display the demo");
        FL_LOG("Fuzzy Control Language:" << std::endl << exporter.toFcl(engine));
        scalar step = 1.0 / 10.0;
        InputVariable* energy = engine->getInputVariable("Energy");
        OutputVariable* health = engine->getOutputVariable("Health");
        for (scalar input = energy->minimum();
                input <= energy->maximum() + step; input += step) {
            energy->setInput(input);
            engine->process();
            health->defuzzify();
        }
    }

    void Example1::main() {
        fl::Example1 sm;
        FL_LOG("Example: " << sm.name());
        FL_LOG("========");
        sm.create();
        sm.test();
    }
} /* namespace fl */

