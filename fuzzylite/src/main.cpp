/*
 * main.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/Headers.h"

#include <typeinfo>
#include <unistd.h>

using namespace fl;

int main(int argc, char** argv) {
    FL_LOG("Hello, fuzzylite!");
    FL_LOG("Version: " << FL_VERSION);
    scalar someScalar = 0;
    int scalarSize = sizeof (someScalar);
#ifdef FL_USE_SINGLE_PRECISION
    FL_LOG("scalar is defined as type <" << (typeid (scalar).name()) << "> (float) with " << scalarSize << " bytes");
#else
    FL_LOG("scalar is defined as type <" << (typeid (scalar).name()) << "> (double) with " << scalarSize << " bytes");
#endif
    FL_LOG("tolerance to floating-point value is " << FL_EPSILON);
    FL_LOG("FL_DEBUG flag is set to: " << FL_DEBUG);

    FL_LOG("An example will start running in 3 seconds...\n");
    if (not FL_DEBUG)
        FL_LOG("No output is shown because fuzzylite was not compiled with "
            "preprocessor definition -DFL_DEBUG=true \n");
    else sleep(3);

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

    engine->configure("Minimum", "Maximum", "Minimum", "Maximum", "Centroid", FL_DEFAULT_DIVISIONS);

    scalar step = 1.0 / 10.0;
    for (scalar input = ambientLight->getMinimum();
            input <= ambientLight->getMaximum() ; input += step) {
        ambientLight->setInput(input);
        engine->process();
        bulbPower->defuzzify();
    }
//    FL_LOG("=============================\n#FCL:\n" << FclExporter().toString(engine) << "\n\n");
//    FL_LOG("=============================\n%FIS:\n" << FisExporter().toString(engine) << "\n\n");

    FL_LOG("Bye, fuzzylite!");

}

