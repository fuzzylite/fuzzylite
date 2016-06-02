/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.
 */

#include "fl/Benchmark.h"

#include "test/catch.hpp"
#include "fl/Headers.h"

#include <vector>
#include <fstream>

namespace fl {

    TEST_CASE("Benchmarks run from Console ", "[benchmark][console]") {
        fuzzylite::setLogging(true);
        const char* args[] = {"dummy-command", "benchmarks", "../examples/", "1"};
        Console::main(4, args);
    }

    TEST_CASE("Benchmarks from FLD files", "[benchmark][fld]") {
        fuzzylite::setLogging(true);
        std::string path = "../examples/original/";
        typedef std::pair<std::string, int > Example;
        std::vector<Example> examples;
        examples.push_back(Example("mamdani/AllTerms", int(1e4)));
        examples.push_back(Example("mamdani/SimpleDimmer", int(1e5)));
        examples.push_back(Example("mamdani/matlab/mam21", 128));
        examples.push_back(Example("mamdani/matlab/mam22", 128));
        examples.push_back(Example("mamdani/matlab/shower", 256));
        examples.push_back(Example("mamdani/matlab/tank", 256));
        examples.push_back(Example("mamdani/matlab/tank2", 512));
        examples.push_back(Example("mamdani/matlab/tipper", 256));
        examples.push_back(Example("mamdani/matlab/tipper1", int(1e5)));
        examples.push_back(Example("mamdani/octave/investment_portfolio", 256));
        examples.push_back(Example("mamdani/octave/mamdani_tip_calculator", 256));
        examples.push_back(Example("takagi-sugeno/approximation", int(1e6)));
        examples.push_back(Example("takagi-sugeno/SimpleDimmer", int(2e6)));
        examples.push_back(Example("takagi-sugeno/matlab/fpeaks", 512));
        examples.push_back(Example("takagi-sugeno/matlab/invkine1", 256));
        examples.push_back(Example("takagi-sugeno/matlab/invkine2", 256));
        examples.push_back(Example("takagi-sugeno/matlab/juggler", 512));
        examples.push_back(Example("takagi-sugeno/matlab/membrn1", 1024));
        examples.push_back(Example("takagi-sugeno/matlab/membrn2", 512));
        examples.push_back(Example("takagi-sugeno/matlab/slbb", 20));
        examples.push_back(Example("takagi-sugeno/matlab/slcp", 20));
        examples.push_back(Example("takagi-sugeno/matlab/slcp1", 15));
        examples.push_back(Example("takagi-sugeno/matlab/slcpp1", 9));
        examples.push_back(Example("takagi-sugeno/matlab/sltbu_fl", 128));
        examples.push_back(Example("takagi-sugeno/matlab/sugeno1", int(2e6)));
        examples.push_back(Example("takagi-sugeno/matlab/tanksg", 1024));
        examples.push_back(Example("takagi-sugeno/matlab/tippersg", 1024));
        examples.push_back(Example("takagi-sugeno/octave/cubic_approximator", int(2e6)));
        examples.push_back(Example("takagi-sugeno/octave/heart_disease_risk", 1024));
        examples.push_back(Example("takagi-sugeno/octave/linear_tip_calculator", 1024));
        examples.push_back(Example("takagi-sugeno/octave/sugeno_tip_calculator", 512));
        examples.push_back(Example("tsukamoto/tsukamoto", int(1e6)));

        std::ostringstream writer;
        writer << "\n" << Benchmark().header(1, "\t") << "\n";
        for (std::size_t i = 0; i < examples.size(); ++i) {
            Example example = examples.at(i);
            FL_LOG("Benchmark " << (i + 1) << "/" << examples.size() << ": "
                    << example.first << ".fll (" << example.second << " values)");

            FL_unique_ptr<Engine> engine(FllImporter().fromFile(path + example.first + ".fll"));

            Benchmark benchmark(example.first, engine.get());
            //            benchmark.prepare(example.second, FldExporter::AllVariables);
            std::ifstream reader(std::string(path +  "../" + example.first + ".fld").c_str());
            benchmark.prepare(reader);
            benchmark.run(1);

            CHECK(benchmark.numberOfErrors() == 0);

            writer << benchmark.results() << "\n";
        }
        FL_LOG(writer.str());
    }

}
