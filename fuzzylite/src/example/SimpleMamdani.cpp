/*
 * SimpleMamdani.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/example/SimpleMamdani.h"

namespace fl {

    SimpleMamdani::SimpleMamdani()
            : _engine(NULL) {
    }

    SimpleMamdani::~SimpleMamdani() {
        if (_engine) delete _engine;
    }

    std::string SimpleMamdani::name() const {
        return "SimpleMamdani";
    }

    Engine* SimpleMamdani::engine() const {
        return this->_engine;
    }

    void SimpleMamdani::create() {
        _engine = new Engine("simple-mamdani");

        InputVariable* energy = new InputVariable("Energy");
        energy->addTerm(new Triangle("LOW", .0, .25, .5));
        energy->addTerm(new Triangle("MEDIUM", .25, .5, .75));
        energy->addTerm(new Triangle("HIGH", .5, .75, 1));
        _engine->addInputVariable(energy);

        OutputVariable* health = new OutputVariable("Health", std::numeric_limits<
                scalar>::quiet_NaN());
        health->addTerm(new Triangle("BAD", 0.0, 0.5, 1));
        health->addTerm(new Triangle("REGULAR", 0.5, 1, 1.5));
        health->addTerm(new Triangle("GOOD", 1, 1.5, 2));
        _engine->addOutputVariable(health);

        RuleBlock* ruleblock = new RuleBlock();
        ruleblock->addRule(MamdaniRule::parse("if Energy is LOW then Health is BAD", _engine));
        ruleblock->addRule(MamdaniRule::parse("if Energy is MEDIUM then Health is REGULAR", _engine));
        ruleblock->addRule(MamdaniRule::parse("if Energy is HIGH then Health is GOOD", _engine));

        _engine->addRuleblock(ruleblock);

        _engine->configure(new Configuration);
    }

    void SimpleMamdani::test() {
        FclExporter exporter;
        FL_LOG("Make sure fuzzylite was compiled with -DFL_DEBUG=true to display the demo");
        FL_LOG("Fuzzy Control Language:" << std::endl << exporter.toFcl(this->engine()));
        scalar step = 1.0 / 10.0;
        InputVariable* energy = _engine->getInputVariable("Energy");
        OutputVariable* health = _engine->getOutputVariable("Health");
        for (scalar input = energy->minimum();
                input <= energy->maximum() + step; input += step) {
            energy->setInput(input);
            _engine->process();
            health->defuzzify();
        }
    }

    void SimpleMamdani::main() {
        fl::SimpleMamdani sm;
        FL_LOG("Example: " << sm.name());
        FL_LOG("========");
        sm.create();
        sm.test();
    }
} /* namespace fl */

