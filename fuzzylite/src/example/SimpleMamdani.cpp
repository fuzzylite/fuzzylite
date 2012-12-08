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
        energy->addTerm(new LeftShoulder("LOW", 0.25, 0.5));
        energy->addTerm(new Triangle("MEDIUM", 0.25, 0.5, 0.75));
        energy->addTerm(new RightShoulder("HIGH", 0.5, 0.75));
        _engine->addInputVariable(energy);

        OutputVariable* health = new OutputVariable("Health", std::numeric_limits<scalar>::quiet_NaN());
        health->addTerm(new Triangle("BAD", 0.0, 0.25, 0.5));
        health->addTerm(new Triangle("REGULAR", 0.25, 0.5, 0.75));
        health->addTerm(new Triangle("GOOD",0.5, 0.75, 1.0));
        _engine->addOutputVariable(health);

        RuleBlock* ruleblock = new RuleBlock();
        ruleblock->addRule(MamdaniRule::parse("if Energy is LOW then Health is BAD", _engine));
        ruleblock->addRule(MamdaniRule::parse("if Energy is MEDIUM then Health is REGULAR", _engine));
        ruleblock->addRule(MamdaniRule::parse("if Energy is HIGH then Health is GOOD", _engine));

        _engine->addRuleblock(ruleblock);

        _engine->configure(new Configuration);
    }

    void SimpleMamdani::test(){
        FclExporter exporter;
        FL_LOG("Fuzzy Control Language:" << std::endl << exporter.toFcl(this->engine()));
        scalar step = 1.0/10.0;
        InputVariable* energy =_engine->getInputVariable("Energy");
        OutputVariable* health = _engine->getOutputVariable("Health");
        FL_LOG(energy->toString());
        FL_LOG(health->toString());
        for (scalar input = energy->minimum() ; input <= energy->maximum() + step; input += step){
            energy->setInput(input);
            _engine->process();
            scalar output = health->defuzzify();
            FL_LOG("===========================");
            FL_LOG("Input (" << energy->getName() << "): " << input << ", "
                    << energy->fuzzify(input));
            FL_LOG("Output (" << health->getName() << "): " << output << ", "
                    << health->fuzzify(output));
        }
    }

} /* namespace fl */
