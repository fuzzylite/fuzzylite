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
        //        examples();
        return Console::main(argc, argv);
    } catch (fl::Exception& e) {
        FL_LOG(e.what());
        FL_LOG(e.btCallStack());
        return EXIT_FAILURE;
    }
}

