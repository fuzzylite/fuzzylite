/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved
 
 This file is part of fuzzylite.
 
 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.
 
 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
 
 fuzzylite™ is a trademark of FuzzyLite Limited.
 
 */

#include "fl/Headers.h"

int main(int argc, char* argv[]){
    using namespace fl;
    Engine* engine = new Engine("simple-dimmer-android");

    InputVariable* ambient = new InputVariable;
    ambient->setName("Ambient");
    ambient->setRange(0.000, 1.000);
    ambient->addTerm(new Triangle("DARK", 0.000, 0.500));
    ambient->addTerm(new Triangle("MEDIUM", 0.250, 0.750));
    ambient->addTerm(new Triangle("BRIGHT", 0.500, 1.000));
    engine->addInputVariable(ambient);

    OutputVariable* power = new OutputVariable;
    power->setName("Power");
    power->setRange(0.000, 2.000);
    power->setDefaultValue(fl::nan);
    power->addTerm(new Triangle("LOW", 0.000, 1.000));
    power->addTerm(new Triangle("MEDIUM", 0.500, 1.500));
    power->addTerm(new Triangle("HIGH", 1.000, 2.000));
    engine->addOutputVariable(power);

    RuleBlock* ruleblock = new RuleBlock;
    ruleblock->addRule(Rule::parse("if Ambient is DARK then Power is HIGH", engine));
    ruleblock->addRule(Rule::parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
    ruleblock->addRule(Rule::parse("if Ambient is BRIGHT then Power is LOW", engine));
    engine->addRuleBlock(ruleblock);

    engine->configure("", "", "Minimum", "Maximum", "Centroid");

    std::string status;
    if (not engine->isReady(&status))
        throw Exception("Engine not ready. "
            "The following errors were encountered:\n" + status, FL_AT);

    for (int i = 0; i < 50; ++i){
        scalar light = ambient->getMinimum() + i * (ambient->range() / 50);
        ambient->setValue(light);
        engine->process();
        FL_LOG("Ambient.input = " << Op::str(light) << " -> " <<
            "Power.output = " << Op::str(power->getValue()));
    }
}
