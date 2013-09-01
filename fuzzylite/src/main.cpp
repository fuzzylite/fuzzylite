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
#include <cstdlib>
#include <signal.h>
#include <fstream>

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

    InputVariable* ambient = new InputVariable("Ambient", 0, 1);
    ambient->addTerm(new Triangle("DARK", .0, .25, .5));
    ambient->addTerm(new Triangle("MEDIUM", .25, .5, .75));
    ambient->addTerm(new Triangle("BRIGHT", .5, .75, 1));
    engine->addInputVariable(ambient);


    OutputVariable* power = new OutputVariable("Power", 0, 2);
    power->setDefaultValue(fl::nan);
    power->addTerm(new Triangle("LOW", 0.0, 0.5, 1));
    power->addTerm(new Triangle("MEDIUM", 0.5, 1, 1.5));
    power->addTerm(new Triangle("HIGH", 1, 1.5, 2));
    engine->addOutputVariable(power);

    RuleBlock* ruleblock = new RuleBlock();
    ruleblock->addRule(FuzzyRule::parse("if Ambient is DARK then Power is HIGH", engine));
    ruleblock->addRule(FuzzyRule::parse("if Ambient is MEDIUM then Power is MEDIUM", engine));
    ruleblock->addRule(FuzzyRule::parse("if Ambient is BRIGHT then Power is LOW", engine));

    engine->addRuleBlock(ruleblock);

    engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid", FL_DIVISIONS);


    std::cout << FclExporter().toString(engine) << "\n";

    std::cout << "Press Enter to test the example..." << std::endl;
    std::cin.get();
    std::cout << "==================================\n\n";

    fl::scalar range = ambient->getMaximum() - ambient->getMinimum();
    int steps = 50;
    for (int i = 0; i < steps; ++i) {
        fl::scalar light = ambient->getMinimum() + i * (range / steps);
        ambient->setInput(light);
        engine->process();
        std::cout << "Ambient.input = " << fl::Op::str(light) << " -> " <<
                "BulbPower.output = " << fl::Op::str(power->defuzzify()) << std::endl;
    }
}

void exampleTakagiSugeno() {
    Engine* engine = new Engine("sin(x)/x");

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
    //    fx->addTerm(Linear::create("f10", engine->inputVariables(), 1.5, 2.4));

    fx->setDefaultValue(fl::nan);

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
    //block->addRule(fl::FuzzyRule::parse("if x is NEAR_9 then fx = f10", engine));

    engine->addRuleBlock(block);

    engine->configure("AlgebraicProduct", "AlgebraicSum", "AlgebraicProduct",
            "", "WeightedAverage", FL_DIVISIONS);

    std::cout << FisExporter().toString(engine) << "\n" << std::endl;

    std::cout << "Press Enter to test the example..." << std::endl;
    std::cin.get();
    std::cout << "==================================\n\n" << std::endl;

    int n = 50;
    scalar mse = 0;
    std::ostringstream r;
    r << "x = c(";
    scalar range = x->getMaximum() - x->getMinimum();
    int steps = 50;
    for (int i = 0; i < steps; ++i) {
        fl::scalar xValue = x->getMinimum() + i * (range / steps);
        x->setInput(xValue);
        engine->process();
        scalar expected = std::sin(xValue) / xValue;
        scalar out = fx->defuzzify();
        scalar se = (expected - out) * (expected - out);
        if (not fl::Op::isNan(se)) {
            mse += se;
            r << fl::Op::str(out) << ", ";
        }

        FL_LOG("x=" << fl::Op::str(xValue)
                << "\tout=" << fl::Op::str(out)
                << "\texpected=" << fl::Op::str(expected)
                << "\tse=" << fl::Op::str(se));
    }
    r << ");";

    FL_LOG("MSE=" << mse / n);

    std::cout << "\n\n";
}

void exampleTakagiSugeno2() {
    //From Octave
    fl::Engine* engine = new fl::Engine;
    engine->setName("Cubic-Approximator");
    engine->addHedge(new fl::Any);
    engine->addHedge(new fl::Extremely);
    engine->addHedge(new fl::Not);
    engine->addHedge(new fl::Seldom);
    engine->addHedge(new fl::Somewhat);
    engine->addHedge(new fl::Very);

    fl::InputVariable* inputVariable1 = new fl::InputVariable;
    inputVariable1->setName("X");
    inputVariable1->setRange(-5.000, 5.000);

    inputVariable1->addTerm(new fl::Triangle("About-Neg-Five", -6.000, -5.000, -4.000));
    inputVariable1->addTerm(new fl::Triangle("About-Neg-Four", -5.000, -4.000, -3.000));
    inputVariable1->addTerm(new fl::Triangle("About-Neg-Three", -4.000, -3.000, -2.000));
    inputVariable1->addTerm(new fl::Triangle("About-Neg-Two", -3.000, -2.000, -1.000));
    inputVariable1->addTerm(new fl::Triangle("About-Neg-One", -2.000, -1.000, 0.000));
    inputVariable1->addTerm(new fl::Triangle("About-Zero", -1.000, 0.000, 1.000));
    inputVariable1->addTerm(new fl::Triangle("About-One", 0.000, 1.000, 2.000));
    inputVariable1->addTerm(new fl::Triangle("About-Two", 1.000, 2.000, 3.000));
    inputVariable1->addTerm(new fl::Triangle("About-Three", 2.000, 3.000, 4.000));
    inputVariable1->addTerm(new fl::Triangle("About-Four", 3.000, 4.000, 5.000));
    inputVariable1->addTerm(new fl::Triangle("About-Five", 4.000, 5.000, 6.000));
    engine->addInputVariable(inputVariable1);

    fl::OutputVariable* outputVariable1 = new fl::OutputVariable;
    outputVariable1->setName("Approx-X-Cubed");
    outputVariable1->setRange(-5.000, 5.000);
    outputVariable1->setDefaultValue(fl::nan);
    outputVariable1->setDefuzzifier(new fl::WeightedAverage());
    outputVariable1->output()->setAccumulation(new fl::Maximum);

    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Neg-Five", engine->inputVariables(), 75.000, 250.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Neg-Four", engine->inputVariables(), 48.000, 128.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Neg-Three", engine->inputVariables(), 27.000, 54.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Neg-Two", engine->inputVariables(), 12.000, 16.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Neg-One", engine->inputVariables(), 3.000, 2.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Zero", engine->inputVariables(), 0.000, 0.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-One", engine->inputVariables(), 3.000, -2.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Two", engine->inputVariables(), 12.000, -16.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Three", engine->inputVariables(), 27.000, -54.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Four", engine->inputVariables(), 48.000, -128.000));
    outputVariable1->addTerm(fl::Linear::create("Tangent-at-Five", engine->inputVariables(), 75.000, -250.000));
    engine->addOutputVariable(outputVariable1);

    fl::RuleBlock* ruleblock1 = new fl::RuleBlock;
    ruleblock1->setName("");
    ruleblock1->setTnorm(new fl::Minimum);
    ruleblock1->setSnorm(new fl::Maximum);
    ruleblock1->setActivation(new fl::Minimum);

    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Neg-Five then Approx-X-Cubed is Tangent-at-Neg-Five", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Neg-Four then Approx-X-Cubed is Tangent-at-Neg-Four", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Neg-Three then Approx-X-Cubed is Tangent-at-Neg-Three", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Neg-Two then Approx-X-Cubed is Tangent-at-Neg-Two", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Neg-One then Approx-X-Cubed is Tangent-at-Neg-One", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Zero then Approx-X-Cubed is Tangent-at-Zero", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-One then Approx-X-Cubed is Tangent-at-One", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Two then Approx-X-Cubed is Tangent-at-Two", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Three then Approx-X-Cubed is Tangent-at-Three", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Four then Approx-X-Cubed is Tangent-at-Four", engine));
    ruleblock1->addRule(fl::FuzzyRule::parse("if X is About-Five then Approx-X-Cubed is Tangent-at-Five", engine));
    engine->addRuleBlock(ruleblock1);

    scalar range = inputVariable1->getMaximum() - inputVariable1->getMinimum();
    int steps = 10;
    for (int i = 0; i < steps; ++i) {
        fl::scalar light = inputVariable1->getMinimum() + i * (range / steps);
        inputVariable1->setInput(light);
        engine->process();
        std::cout << "x.input = " << fl::Op::str(light) << " -> " <<
                "x^3.output = " << fl::Op::str(outputVariable1->defuzzify()) << std::endl;
    }
}

void exportAllExamples(const std::string& from, const std::string& to) {
    std::vector<std::string> examples;
    examples.push_back("/mamdani/AllTerms");
    examples.push_back("/mamdani/SimpleDimmer");
    examples.push_back("/mamdani/matlab/mam21");
    examples.push_back("/mamdani/matlab/mam22");
    examples.push_back("/mamdani/matlab/shower");
    examples.push_back("/mamdani/matlab/tank");
    examples.push_back("/mamdani/matlab/tank2");
    examples.push_back("/mamdani/matlab/tipper");
    examples.push_back("/mamdani/matlab/tipper1");
    examples.push_back("/mamdani/octave/investment_portfolio");
    examples.push_back("/mamdani/octave/mamdani_tip_calculator");
    examples.push_back("/takagi-sugeno/SimpleDimmer");
    examples.push_back("/takagi-sugeno/matlab/fpeaks");
    examples.push_back("/takagi-sugeno/matlab/invkine1");
    examples.push_back("/takagi-sugeno/matlab/invkine2");
    examples.push_back("/takagi-sugeno/matlab/juggler");
    examples.push_back("/takagi-sugeno/matlab/membrn1");
    examples.push_back("/takagi-sugeno/matlab/membrn2");
    examples.push_back("/takagi-sugeno/matlab/slbb");
    examples.push_back("/takagi-sugeno/matlab/slcp");
    examples.push_back("/takagi-sugeno/matlab/slcp1");
    examples.push_back("/takagi-sugeno/matlab/slcpp1");
    examples.push_back("/takagi-sugeno/matlab/sltbu-fl");
    examples.push_back("/takagi-sugeno/matlab/sugeno1");
    examples.push_back("/takagi-sugeno/matlab/tanksg");
    examples.push_back("/takagi-sugeno/matlab/tippersg");
    examples.push_back("/takagi-sugeno/octave/cubic_approximator");
    examples.push_back("/takagi-sugeno/octave/heart_disease_risk");
    examples.push_back("/takagi-sugeno/octave/linear_tip_calculator");
    examples.push_back("/takagi-sugeno/octave/sugeno_tip_calculator");

    std::string sourceBase = "/home/jcrada/Development/fuzzylite/examples/" + from;
    std::string targetBase = "/home/jcrada/Development/fuzzylite/examples/" + to;

    Importer* importer;
    if (from == "fis") importer = new FisImporter;
    else if (from == "fcl") importer = new FclImporter;
    else throw fl::Exception("[examples error] unrecognized format <" + from + "> to import", FL_AT);

    Exporter* exporter;
    if (to == "cpp") exporter = new CppExporter;
    else if (to == "fis") exporter = new FisExporter;
    else if (to == "fcl") exporter = new FclExporter;
    else throw fl::Exception("[examples error] unrecognized format <" + to + "> to export", FL_AT);

    std::ostringstream errors;
    for (std::size_t i = 0; i < examples.size(); ++i) {
        try {
            std::ostringstream ss;
            std::string input = sourceBase + examples.at(i) + "." + from;
            std::ifstream source(input.c_str());
            if (source.is_open()) {
                std::string line;
                while (source.good()) {
                    std::getline(source, line);
                    ss << line << "\n";
                }
                source.close();
            } else throw fl::Exception("[examples error] file not found: " + input, FL_AT);

            Engine* engine = importer->fromString(ss.str());
            {//Test FCL
                std::string fcl = FclExporter().toString(engine);
                Engine* fclEngine = FclImporter().fromString(fcl);
                std::string copy = FclExporter().toString(fclEngine);
                delete fclEngine;
                if (fcl != copy) {
                    errors << "[fcl error] at " + examples.at(i) + "." + from + ":\n";
                    errors << fcl << "\n\n" << copy << "\n";
                }
            }

            {
                //Test FIS
                std::string fis = FisExporter().toString(engine);
                Engine* fisEngine = FisImporter().fromString(fis);
                std::string copy = FisExporter().toString(fisEngine);
                delete fisEngine;
                if (fis != copy) {
                    errors << "[fis error] at " + examples.at(i) + "." + from;
                }
            }

            std::string output = targetBase + examples.at(i) + "." + to;
            std::ofstream target(output.c_str());
            if (target.is_open()) {
                if (to == "cpp") {
                    target << "#include <fl/Headers.h>\n\n";
                    target << "int main(int argc, char** argv){\n";
                    target << exporter->toString(engine);
                    target << "\n}\n";
                } else {
                    target << exporter->toString(engine);
                }
            }
            target.close();
            delete engine;
        } catch (fl::Exception& ex) {
            errors << "error at " << examples.at(i) << ":\n" << ex.what() << "\n";
        }
    }
    delete importer;
    delete exporter;
    if (errors.str().empty()) {
        FL_LOG("No errors were found exporting files");
    } else {
        FL_LOG("The following errors occured while exporting files:" << errors.str());
    }
}

void examples() throw (fl::Exception) {
    std::cout << "\nHello, " << fl::fuzzylite::name() << "!\n"
            << "=================\n"
            << "Version " << fl::fuzzylite::longVersion() << "\n"
            << "Built on " << fl::fuzzylite::platform() << " "
            << "in " << fl::fuzzylite::configuration() << " mode\n"
            << "Floating-point " << fl::fuzzylite::floatingPoint() << "\n"
            << "Precision is set to " << fl::fuzzylite::precision() << "\n"
            << "Only " << fl::fuzzylite::decimals() << " decimals are printed\n"
            << "Infinity is printed as: (" << fl::Op::str(-fl::inf)
            << " , " << fl::Op::str(fl::inf) << ")\n"
            << "NaN values are printed as: " << fl::Op::str(fl::nan) << "\n"
            << "Defuzzifiers by default use " << fl::fuzzylite::defaultDivisions() << " divisions\n";
    if (fl::fuzzylite::logEnabled()) {
        std::cout << "FL_LOG is enabled and prints in console as follows:\n";
        FL_LOG("message from FL_LOG");
        std::cout << "where " << FL_LOG_PREFIX << " indicates the origin of the call\n";
    } else {
        std::cout << "FL_LOG is NOT enabled and hence will print nothing\n";
    }

    std::cout << "\nPress Enter to continue with a Mamdani example..." << std::endl;
    std::cin.get();
    std::cout << "\n==========================================\n";

    exampleMamdani();

    std::cout << "\nPress Enter to continue with a Takagi-Sugeno example..." << std::endl;
    std::cin.get();
    std::cout << "\n==========================================\n";

    exampleTakagiSugeno();
    //    exampleTakagiSugeno2();

    std::cout << "Bye, " << fl::fuzzylite::name() << "!\n\n";
    std::cout << "Please visit http://www.fuzzylite.com\n\n"
            "...and consider making a *donation* to support this project.\n\n"
            "There are still many things to do!\n\n";

}

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;
    std::set_terminate(fl::Exception::terminate);
    std::set_unexpected(fl::Exception::terminate);
    signal(SIGSEGV, fl::Exception::signalHandler);
    signal(SIGABRT, fl::Exception::signalHandler);
    signal(SIGILL, fl::Exception::signalHandler);
    signal(SIGSEGV, fl::Exception::signalHandler);
    signal(SIGFPE, fl::Exception::signalHandler);
#ifdef FL_UNIX
    signal(SIGBUS, fl::Exception::signalHandler);
    signal(SIGPIPE, fl::Exception::signalHandler);
#endif
    
    try {
//        exportAllExamples("fis", "cpp");
//        return 0;
        examples();
    } catch (fl::Exception& e) {
        FL_LOG(e.what());
        FL_LOG(e.btCallStack());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

