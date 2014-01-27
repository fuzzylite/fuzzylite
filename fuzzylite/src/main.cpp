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
    examples.push_back("/takagi-sugeno/approximation");
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
    examples.push_back("/takagi-sugeno/matlab/sltbu_fl");
    examples.push_back("/takagi-sugeno/matlab/sugeno1");
    examples.push_back("/takagi-sugeno/matlab/tanksg");
    examples.push_back("/takagi-sugeno/matlab/tippersg");
    examples.push_back("/takagi-sugeno/octave/cubic_approximator");
    examples.push_back("/takagi-sugeno/octave/heart_disease_risk");
    examples.push_back("/takagi-sugeno/octave/linear_tip_calculator");
    examples.push_back("/takagi-sugeno/octave/sugeno_tip_calculator");
    examples.push_back("/tsukamoto/tsukamoto");

    std::string sourceBase = "/home/jcrada/Development/fl/fuzzylite/examples";
    //    std::string targetBase = "/home/jcrada/Development/fuzzylite/examples/" + to;
    std::string targetBase = "/tmp/fl/";

    Importer* importer;
    if (from == "fll") importer = new FllImporter;
    else if (from == "fis") importer = new FisImporter;
    else if (from == "fcl") importer = new FclImporter;
    else throw fl::Exception("[examples error] unrecognized format <" + from + "> to import", FL_AT);

    Exporter* exporter;
    if (to == "fll") exporter = new FllExporter;
    else if (to == "fld") exporter = new FldExporter(" ", 1024);
    else if (to == "fcl") exporter = new FclExporter;
    else if (to == "fis") exporter = new FisExporter;
    else if (to == "cpp") exporter = new CppExporter;
    else if (to == "java") exporter = new JavaExporter;
    else throw fl::Exception("[examples error] unrecognized format <" + to + "> to export", FL_AT);

    std::vector<std::pair<Exporter*, Importer*> > tests;
    tests.push_back(std::pair<Exporter*, Importer*>(new FllExporter, new FllImporter));
    tests.push_back(std::pair<Exporter*, Importer*>(new FclExporter, new FclImporter));
    tests.push_back(std::pair<Exporter*, Importer*>(new FisExporter, new FisImporter));
    std::ostringstream errors;
    for (std::size_t i = 0; i < examples.size(); ++i) {
        FL_LOG("Processing " << (i + 1) << "/" << examples.size() << ": " << examples.at(i));
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

            for (std::size_t t = 0; t < tests.size(); ++t) {
                std::string out = tests.at(t).first->toString(engine);
                Engine* copy = tests.at(t).second->fromString(out);
                std::string out_copy = tests.at(t).first->toString(copy);

                if (out != out_copy) {
                    errors << "[imex error] different results <"
                            << importer->name() << "," << exporter->name() << "> "
                            "at " + examples.at(t) + "." + from + ":\n";
                }
            }

            std::string output = targetBase + examples.at(i) + "." + to;
            std::ofstream target(output.c_str());
            if (target.is_open()) {
                if (to == "cpp") {
                    target << "#include <fl/Headers.h>\n\n"
                            << "int main(int argc, char** argv){\n"
                            << exporter->toString(engine)
                            << "\n}\n";
                } else if (to == "java") {
                    std::string className = examples.at(i).substr(examples.at(i).find_last_of('/') + 1);
                    target << "import com.fuzzylite.*;\n"
                            << "import com.fuzzylite.defuzzifier.*;\n"
                            << "import com.fuzzylite.factory.*;\n"
                            << "import com.fuzzylite.hedge.*;\n"
                            << "import com.fuzzylite.imex.*;\n"
                            << "import com.fuzzylite.norm.*;\n"
                            << "import com.fuzzylite.norm.s.*;\n"
                            << "import com.fuzzylite.norm.t.*;\n"
                            << "import com.fuzzylite.rule.*;\n"
                            << "import com.fuzzylite.term.*;\n"
                            << "import com.fuzzylite.variable.*;\n\n"
                            << "public class " << Op::makeValidId(className) << "{\n"
                            << "public static void main(String[] args){\n"
                            << exporter->toString(engine)
                            << "\n}\n}\n";
                } else {
                    target << exporter->toString(engine);
                }
                target.close();
            }
            delete engine;
        } catch (fl::Exception& ex) {
            errors << "error at " << examples.at(i) << ":\n" << ex.what() << "\n";
        }
    }
    delete importer;
    delete exporter;
    for (std::size_t i = 0; i < tests.size(); ++i) {
        delete tests.at(i).first;
        delete tests.at(i).second;
    }
    if (errors.str().empty()) {
        FL_LOG("No errors were found exporting files");
    } else {
        FL_LOG("The following errors were encountered while exporting files:" << errors.str());
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
//        fuzzylite::setDecimals(3);
//        exportAllExamples("fis", "fll");
//        exportAllExamples("fis", "fcl");
//        exportAllExamples("fis", "fis");
//        exportAllExamples("fis", "cpp");
//        exportAllExamples("fis", "java");
//        fuzzylite::setDecimals(8);
//        exportAllExamples("fis", "fld");
//        return 0;
        return Console::main(argc, argv);
    } catch (fl::Exception& e) {
        FL_LOG(e.what());
        FL_LOG(e.btCallStack());
        return EXIT_FAILURE;
    }
}

