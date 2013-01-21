/*
 * main.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/Headers.h"

#include <typeinfo>
#include <iomanip>

using namespace fl;

int main(int argc, char** argv) {
    std::cout << "\nHello, fuzzylite!" << std::endl
            << "Version: " << FL_VERSION << " (" << FL_DATE << ")\n" << std::endl;
    scalar someScalar = 0;
    int scalarSize = sizeof (someScalar);
    (void) scalarSize;
#ifdef FL_USE_FLOAT
    std::cout << "scalar is defined as type <" << (typeid (scalar).name()) << "> "
            "(float) with " << scalarSize << " bytes" << std::endl;
#else
    std::cout << "scalar is defined as type <" << (typeid (scalar).name()) << "> "
            "(double) with " << scalarSize << " bytes\n";
#endif
    std::cout << "tolerance to floating-point value is " << FL_PRECISION
            << " (FL_PRECISION)" << std::endl
            << FL_DECIMALS << " decimals are printed (FL_DECIMALS)" << std::endl;
#ifdef FL_NO_LOG
    std::cout << "Macro FL_LOG will not print anything because of directive -DFL_NO_LOG\n";
#else
    std::cout << "FL_LOG is enabled\n";
#endif
    if (FL_DEBUG)
        std::cout << "FL_DEBUG is enabled. Use with CARE. Very detailed information will be printed to console "
            "everytime the fl::Engine::process() is called. NOT good for performance!\n"
            << std::endl;
    else
        std::cout << "FL_DEBUG is disabled.\nFL_DBG and FL_BEGIN_BLOCKs are disabled.\n";

    std::cout << std::setprecision(FL_DECIMALS) << std::fixed;

    std::cout << "infinity values are printed as: " << std::numeric_limits<scalar>::infinity() << "\n"
            << "NaN values are printed as: " << std::numeric_limits<scalar>::quiet_NaN() << "\n";



    Engine* engine = new Engine("simple-dimmer");

    InputVariable* ambientLight = new InputVariable("AmbientLight", 0, 1);
    ambientLight->addTerm(new Triangle("LOW", .0, .25, .5));
    ambientLight->addTerm(new Triangle("MEDIUM", .25, .5, .75));
    ambientLight->addTerm(new Triangle("HIGH", .5, .75, 1));
    engine->addInputVariable(ambientLight);


    OutputVariable* bulbPower = new OutputVariable("BulbPower", 0, 2);
    bulbPower->setDefaultValue(std::numeric_limits<scalar>::quiet_NaN());
    bulbPower->setLockDefuzzifiedValue(true);
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

    std::cout << "\nPress Enter to continue with an example..." << std::endl;
    std::cin.get();
    std::cout << "\n\n" << FclExporter().toString(engine) << "\n" << std::endl;

    std::cout << "Press Enter to continue with the example..." << std::endl;
    std::cin.get();

    scalar step = 1.0 / 25.0;
    for (scalar input = ambientLight->getMinimum();
            input <= ambientLight->getMaximum(); input += step) {
        ambientLight->setInput(input);
        std::cout << "AmbientLight.input=" << input << " -> ";
        engine->process();
        std::cout << "BulbPower.output.defuzzify=" << bulbPower->defuzzify() << std::endl;
    }

    std::cout << "\n\n";

    std::cout << "Bye, fuzzylite!\n" << std::endl;
//    std::cin.get();

}

