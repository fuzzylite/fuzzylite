/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * main.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/Headers.h"

#include <typeinfo>
#include <iomanip>

#include <signal.h>

using namespace fl;

/*
void baz(){
    int *x = (int*) - 1; // make a bad pointer
    FL_LOG("%d\n" <<  *x); // causes segfault
}

void bar(){
    baz();
}

void foo(){
    bar();
}
 */

int main(int argc, char** argv) {
    signal(SIGSEGV, fl::Exception::signalHandler);
    signal(SIGABRT, fl::Exception::signalHandler);
    signal(SIGILL, fl::Exception::signalHandler);
    signal(SIGSEGV, fl::Exception::signalHandler);
    signal(SIGFPE, fl::Exception::signalHandler);
#ifdef FL_UNIX
    signal(SIGBUS, fl::Exception::signalHandler);
    signal(SIGPIPE, fl::Exception::signalHandler);
#endif
    // foo();
    std::cout << "\nHello, " << fl::fuzzylite::name() << "!\n"
            << "=================\n"
            << "Version " << fl::fuzzylite::longVersion() << "\n"
            << "Built on " << fl::fuzzylite::platform() << " "
            << "in " << fl::fuzzylite::configuration() << " mode\n"
            << "Floating-point " << fl::fuzzylite::floatingPoint() << "\n"
            << "Precision is set to " << fl::fuzzylite::precision() << "\n"
            << "Only " << fl::fuzzylite::decimals() << " decimals are printed\n"
            << std::setprecision(FL_DECIMALS) << std::fixed
            << "Infinity is printed as: (" << (-std::numeric_limits<scalar>::infinity())
            << " , " << std::numeric_limits<scalar>::infinity() << ")\n"
            << "NaN values are printed as: " << std::numeric_limits<scalar>::quiet_NaN() << "\n"
            << "Defuzzifiers by default use " << fl::fuzzylite::defaultDivisions() << " divisions\n";
    if (fl::fuzzylite::logEnabled()) {
        std::cout << "FL_LOG is enabled and prints in console as follows:\n";
        FL_LOG("message from FL_LOG");
        std::cout << "where " << FL_LOG_PREFIX << " indicates the origin of the call\n";
    } else {
        std::cout << "FL_LOG is NOT enabled and hence will not print anything\n";
    }

    std::cout << "\nPress Enter to continue with an example..." << std::endl;
    std::cin.get();
    std::cout << "\n==========================================\n";

    Engine* engine = new Engine("simple-dimmer");

    InputVariable* ambientLight = new InputVariable("AmbientLight", 0, 1);
    ambientLight->addTerm(new Triangle("LOW", .0, .25, .5));
    ambientLight->addTerm(new Triangle("MEDIUM", .25, .5, .75));
    ambientLight->addTerm(new Triangle("HIGH", .5, .75, 1));
    engine->addInputVariable(ambientLight);


    OutputVariable* bulbPower = new OutputVariable("BulbPower", 0, 2);
    bulbPower->setDefaultValue(std::numeric_limits<scalar>::quiet_NaN());
    bulbPower->setLockDefuzzifiedValue(false);
    bulbPower->addTerm(new Triangle("LOW", 0.0, 0.5, 1));
    bulbPower->addTerm(new Triangle("MEDIUM", 0.5, 1, 1.5));
    bulbPower->addTerm(new Triangle("HIGH", 1, 1.5, 2));
    engine->addOutputVariable(bulbPower);

    RuleBlock* ruleblock = new RuleBlock();
    ruleblock->addRule(MamdaniRule::parse("if AmbientLight is LOW then BulbPower is LOW", engine));
    ruleblock->addRule(MamdaniRule::parse("if AmbientLight is MEDIUM then BulbPower is MEDIUM", engine));
    ruleblock->addRule(MamdaniRule::parse("if AmbientLight is HIGH then BulbPower is HIGH", engine));

    engine->addRuleBlock(ruleblock);

    engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid", FL_DIVISIONS);


    std::cout << FclExporter().toString(engine) << "\n" << std::endl;

    std::cout << "Press Enter to test the example..." << std::endl;
    std::cin.get();
    std::cout << "==================================\n\n" << std::endl;

    scalar step = 1.0 / 25.0;
    for (scalar input = ambientLight->getMinimum();
            input <= ambientLight->getMaximum() + step; input += step) {
        ambientLight->setInput(input);
        std::cout << "AmbientLight.input=" << input << " -> ";
        engine->process();
        std::cout << "BulbPower.output.defuzzify=" << bulbPower->defuzzify() << std::endl;
    }

    std::cout << "\n\n";

    std::cout << "Bye, " << fl::fuzzylite::name() << "!\n\n";
    std::cout << "Please visit http://www.fuzzylite.com\n\n"
            "...and consider making a *donation* to support this project.\n\n"
            "There are still many things to do!\n\n";

}

