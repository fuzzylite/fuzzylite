/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include <fstream>
#include <typeinfo>
#include <vector>

#include "Headers.h"

namespace fuzzylite { namespace test {
    TEST_CASE("Benchmarks run from Console ", "[benchmark][console]") {
        //        const char* args[] = {"dummy-command", "benchmarks", "../../examples/", "1"};
        //        Console::main(4, args);
    }

    TEST_CASE("Benchmarks from FLD files", "[benchmark][fld]") {
#ifdef FL_WINDOWS
        const std::string sep("\\");
#else
        const std::string sep("/");
#endif
        const std::string here(__FILE__);
        std::string test;
        std::size_t index = here.rfind(sep);
        if (index != std::string::npos)
            test = here.substr(0, index + 1);
        const std::string path(test + ".." + sep + "examples" + sep);
        const int test_values = 1024;
        std::vector<std::string> examples;
        examples.push_back("mamdani/AllTerms");
        examples.push_back("mamdani/SimpleDimmer");
        examples.push_back("mamdani/matlab/mam21");
        examples.push_back("mamdani/matlab/mam22");
        examples.push_back("mamdani/matlab/shower");
        examples.push_back("mamdani/matlab/tank");
        examples.push_back("mamdani/matlab/tank2");
        examples.push_back("mamdani/matlab/tipper");
        examples.push_back("mamdani/matlab/tipper1");
        examples.push_back("mamdani/octave/investment_portfolio");
        examples.push_back("mamdani/octave/mamdani_tip_calculator");
        examples.push_back("takagi-sugeno/approximation");
        examples.push_back("takagi-sugeno/SimpleDimmer");
        examples.push_back("takagi-sugeno/matlab/fpeaks");
        examples.push_back("takagi-sugeno/matlab/invkine1");
        examples.push_back("takagi-sugeno/matlab/invkine2");
        examples.push_back("takagi-sugeno/matlab/juggler");
        examples.push_back("takagi-sugeno/matlab/membrn1");
        examples.push_back("takagi-sugeno/matlab/membrn2");
        examples.push_back("takagi-sugeno/matlab/slbb");
        examples.push_back("takagi-sugeno/matlab/slcp");
        examples.push_back("takagi-sugeno/matlab/slcp1");
        examples.push_back("takagi-sugeno/matlab/slcpp1");
        examples.push_back("takagi-sugeno/matlab/sltbu_fl");
        examples.push_back("takagi-sugeno/matlab/sugeno1");
        examples.push_back("takagi-sugeno/matlab/tanksg");
        examples.push_back("takagi-sugeno/matlab/tippersg");
        examples.push_back("takagi-sugeno/octave/cubic_approximator");
        examples.push_back("takagi-sugeno/octave/heart_disease_risk");
        examples.push_back("takagi-sugeno/octave/linear_tip_calculator");
        examples.push_back("takagi-sugeno/octave/sugeno_tip_calculator");
        examples.push_back("tsukamoto/tsukamoto");

        std::vector<int> errors = std::vector<int>(examples.size(), 0);
        for (std::size_t i = 0; i < examples.size(); ++i) {
            const std::string example = examples.at(i);
            const std::string base_file = path + example;
            CAPTURE(example, base_file);
            FL_unique_ptr<Engine> engine(FllImporter().fromFile(base_file + ".fll"));

            const scalar tolerance = typeid(scalar) == typeid(float) ? 1e-3 : fuzzylite::absoluteTolerance();
            Benchmark benchmark(example, engine.get(), tolerance);

            std::ifstream reader(std::string(base_file + ".fld").c_str());
            if (not reader.is_open())
                throw Exception("File not found: " + base_file + ".fld");
            benchmark.prepare(reader, test_values);
            benchmark.run(1);
            CHECK(benchmark.canComputeErrors() == true);
            errors.at(i) = benchmark.accuracyErrors();
        }

        for (std::size_t i = 0; i < errors.size(); ++i) {
            const std::string example = examples.at(i);
            const std::string base_file = path + example;
            CAPTURE(example, path);
            // when built with scalar=float, tsukamoto correctly produces infinity in 44 input cases,
            // whereas such is not the case when scalar=double. Specifically, when iterating over x and:
            // when scalar==float and $y = 1.0$: sigmoid::tsukamoto(y=1)=...log(1.0 / (y - 1.0)) = +Inf.
            // when scalar==double, $y = 0.99999999996$, which prevents infinity.
            if (typeid(scalar) == typeid(float) and example.find("tsukamoto") != std::string::npos)
                CHECK(errors.at(i) == 44);
            else
                CHECK(errors.at(i) == 0);
        }
    }

    TEST_CASE("Time conversions", "[benchmark][time]") {
        CHECK(Op::isEq(1.0, Benchmark::convert(3600, Benchmark::Seconds, Benchmark::Hours)));
        FL_LOG(Benchmark::convert(3600, Benchmark::Seconds, Benchmark::Hours));
        CHECK(Op::isEq(3600, Benchmark::convert(1, Benchmark::Hours, Benchmark::Seconds)));
        FL_LOG(Benchmark::convert(1, Benchmark::Hours, Benchmark::Seconds));

        CHECK(Op::isEq(1000.0, Benchmark::convert(1.0, Benchmark::Seconds, Benchmark::MilliSeconds)));
        FL_LOG(Benchmark::convert(1.0, Benchmark::Seconds, Benchmark::MilliSeconds));
        CHECK(Op::isEq(1.0, Benchmark::convert(1000.0, Benchmark::MilliSeconds, Benchmark::Seconds)));
        FL_LOG(Benchmark::convert(1000.0, Benchmark::MilliSeconds, Benchmark::Seconds));

        CHECK(Op::isEq(35e9, Benchmark::convert(35, Benchmark::Seconds, Benchmark::NanoSeconds)));
        CHECK(Op::isEq(35, Benchmark::convert(35e9, Benchmark::NanoSeconds, Benchmark::Seconds)));
    }

    TEST_CASE("Benchmark headers", "[benchmark][header]") {
        FL_LOG(Op::join(Benchmark().header(10, true), "\t"));
        CHECK(Benchmark().header(10).size() == 30);

        FL_LOG(Op::join(Benchmark().header(10, false), "\t"));
        CHECK(Benchmark().header(10, false).size() == 30 - 8);
    }
}}
