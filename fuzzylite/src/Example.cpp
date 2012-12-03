/*
 * Example.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "Example.h"

#include "Headers.h"

namespace fl {

    void Example::simpleMamdani() {
        Engine* engine = new Engine("simple-mamdani");

        InputVariable* energy = new InputVariable("Energy");
        energy->addTerm(new Triangle("LOW", 0.0, 0.5, 1.0));
        energy->addTerm(new Triangle("MEDIUM", 0.5, 1.0, 1.5));
        energy->addTerm(new Triangle("HIGH", 1.0, 1.5, 2.0));
        engine->addInputVariable(energy);

        OutputVariable* health = new OutputVariable("Health");
        health->addTerm(new LeftShoulder("BAD", 0.0, 0.5));
        health->addTerm(new Triangle("REGULAR", 0.5, 1.0, 1.5));
        health->addTerm(new RightShoulder("GOOD", 1.0, 1.5));
        engine->addOutputVariable(health);

        RuleBlock* ruleblock = new RuleBlock();
//        ruleblock->addRule();
        //                ruleblock = RuleBlock()
        //                ruleblock.append(MamdaniRule.parse('if Energy is LOW then Health is BAD', fe))
        //                ruleblock.append(MamdaniRule.parse('if Energy is MEDIUM then Health is REGULAR', fe))
        //                ruleblock.append(MamdaniRule.parse('if Energy is HIGH then Health is GOOD', fe))
        //                fe.ruleblock[ruleblock.name] = ruleblock
        engine->addRuleblock(ruleblock);

        engine->configure(new Configuration);

    }

} /* namespace fl */
