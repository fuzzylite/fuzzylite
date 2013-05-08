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


void exampleMamdani() {
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
}

void exampleTakagiSugeno() {
    Engine* engine = new Engine("simple-dimmer");

    fl::InputVariable* x = new fl::InputVariable("x");
    x->setRange(0, 10);

    x->addTerm(new fl::Triangle("NEAR_1", 0, 1, 2));
    x->addTerm(new fl::Triangle("NEAR_2", 1, 2, 3));
    x->addTerm(new fl::Triangle("NEAR_3", 2, 3, 4));
    x->addTerm(new fl::Triangle("NEAR_4", 3, 4, 5));
    x->addTerm(new fl::Triangle("NEAR_5", 4, 5, 6));
    x->addTerm(new fl::Triangle("NEAR_6", 5, 6, 7));
    x->addTerm(new fl::Triangle("NEAR_7", 6, 7, 8));
    x->addTerm(new fl::Triangle("NEAR_8", 7, 8, 9));
    x->addTerm(new fl::Triangle("NEAR_9", 8, 9, 10));
    engine->addInputVariable(x);


    fl::OutputVariable* fx = new fl::OutputVariable("fx");
    fx->setRange(-1, 1);
    //    f_x->addTerm(new fl::FunctionTerm("function", "(sin x) / x", 0, 10));
    engine->addOutputVariable(fx);
    fx->addTerm(new Constant("f1", 0.84));
    fx->addTerm(new Constant("f2", 0.45));
    fx->addTerm(new Constant("f3", 0.04));
    fx->addTerm(new Constant("f4", -0.18));
    fx->addTerm(new Constant("f5", -0.19));
    fx->addTerm(new Constant("f6", -0.04));
    fx->addTerm(new Constant("f7", 0.09));
    fx->addTerm(new Constant("f8", 0.12));
    fx->addTerm(new Constant("f9", 0.04));

    //    bulbPower->setDefaultValue(std::numeric_limits<scalar>::quiet_NaN());
    //    bulbPower->setLockDefuzzifiedValue(false);

    fl::RuleBlock* block = new fl::RuleBlock();
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_1 then fx = f1", engine));
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_2 then fx = f2", engine));
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_3 then fx = f3", engine));
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_4 then fx = f4", engine));
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_5 then fx = f5", engine));
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_6 then fx = f6", engine));
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_7 then fx = f7", engine));
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_8 then fx = f8", engine));
    block->addRule(fl::FuzzyRule::parse("if x is NEAR_9 then fx = f9", engine));

    engine->addRuleBlock(block);

    engine->configure("AlgebraicProduct", "AlgebraicSum", "AlgebraicProduct", "Maximum", "Centroid", FL_DIVISIONS);
    fx->setDefuzzifier(new WeightedAverage());

    int n = 5;
    scalar mse = 0;
    for (fl::scalar in = x->getMinimum(); in < x->getMaximum();
            in += (x->getMinimum() + x->getMaximum()) / n) {
        x->setInput(in);
        engine->process();
        scalar expected = 0;
        //        flScalar expected = fx->getTerm(0)->membership(in);
        scalar obtained = fx->defuzzify();
        scalar se = (expected - obtained) * (expected - obtained);
        mse += isnan(se) ? 0 : se;
        FL_LOG("x=" << in << "\texpected_out=" << expected << "\tobtained_out=" << obtained
                << "\tse=" << se);
    }
    FL_LOG("MSE=" << mse / n);

    //    std::cout << FclExporter().toString(engine) << "\n" << std::endl;

    std::cout << "\n\n";
}


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

//    exampleMamdani();
    exampleTakagiSugeno();

    std::cout << "Bye, " << fl::fuzzylite::name() << "!\n\n";
    std::cout << "Please visit http://www.fuzzylite.com\n\n"
            "...and consider making a *donation* to support this project.\n\n"
            "There are still many things to do!\n\n";

}
