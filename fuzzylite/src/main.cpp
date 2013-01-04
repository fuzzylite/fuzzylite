/*
 * main.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/Headers.h"

#include <typeinfo>

#include <unistd.h>
#include <algorithm>
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
    int wait = 3;

    std::vector<Example*> examples;
    examples.push_back(new Example1);
    examples.push_back(new AllTermsExample);
    FL_LOG("The examples will start running now");
    FisExporter exporter;
    FisImporter importer;
    for (std::size_t i = 0; i < examples.size(); ++i) {
        Example* example = examples[i];
        for (int w = 0; w < wait; ++w) {
            FL_LOG("Example <" << example->name() << "> will start running in "
                    << (wait - w) << " seconds...");
            sleep(1);
        }
        std::string fcl = exporter.toString(example->engine);
        FL_LOG(fcl);
        //        Engine* engine = importer.fromString(fcl);
        //        std::string fclAgain = exporter.toString(engine);
        //        if (fcl != fclAgain) {
        //            throw fl::Exception("FCL Importer/Exporter not working", FL_AT);
        //        }

        example->test();
    }

    FL_LOG("Bye, fuzzylite!");

}

