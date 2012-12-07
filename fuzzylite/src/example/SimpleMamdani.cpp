/*
 * SimpleMamdani.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "SimpleMamdani.h"


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
        energy->addTerm(new Triangle("LOW", 0.0, 0.5, 1.0));
        energy->addTerm(new Triangle("MEDIUM", 0.5, 1.0, 1.5));
        energy->addTerm(new Triangle("HIGH", 1.0, 1.5, 2.0));
        _engine->addInputVariable(energy);

        OutputVariable* health = new OutputVariable("Health");
        health->addTerm(new LeftShoulder("BAD", 0.0, 0.5));
        health->addTerm(new Triangle("REGULAR", 0.5, 1.0, 1.5));
        health->addTerm(new RightShoulder("GOOD", 1.0, 1.5));
        _engine->addOutputVariable(health);

        RuleBlock* ruleblock = new RuleBlock();
        ruleblock->addRule(MamdaniRule::parse("if Energy is LOW then Health is BAD", _engine));
        ruleblock->addRule(MamdaniRule::parse("if Energy is MEDIUM then Health is REGULAR", _engine));
        ruleblock->addRule(MamdaniRule::parse("if Energy is HIGH then Health is GOOD", _engine));

        _engine->addRuleblock(ruleblock);

        _engine->configure(new Configuration);
    }

    void SimpleMamdani::test(){
        scalar step = 1.0/10.0;
        InputVariable* energy =_engine->getInputVariable("Energy");
        OutputVariable* health = _engine->getOutputVariable("Health");
        for (scalar input = energy->minimum() ; input <= energy->maximum(); input += step){
            energy->setInput(input);
            _engine->process();
            scalar output = health->defuzzify();
            FL_LOG("===========================");
            FL_LOG("Input (" << energy->getName() << "): " << input << ", " << energy->fuzzify(input));
            FL_LOG("Output (" << health->getName() << "): " << output << ", " << health->fuzzify(output));
        }
    }

} /* namespace fl */
