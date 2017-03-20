/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "test/catch.hpp"
#include "fl/Headers.h"
#include <fstream>

namespace fl {

    TEST_CASE("RExporter creates plots with a single variable", "[imex]") {
        FL_unique_ptr<Engine> engine(Console::mamdani());
        engine->addInputVariable(new InputVariable("Dummy", 0, 1));

        std::string filename("/tmp/simple-dimmer.fld");
        {
            std::ofstream os(filename.c_str());
            FldExporter().write(engine.get(), os, 1024);
            os.close();
        }

        std::ifstream is(filename.c_str());
        std::string dummy;
        std::getline(is, dummy); //remove header

        std::ostringstream os;

        RScriptExporter().writeScriptImportingDataFrame(engine.get(), os,
                engine->getInputVariable(0), engine->getInputVariable(1),
                "/dummy/path/to/x.fld",
                engine->outputVariables());
        FL_LOG(os.str());
    }

    TEST_CASE("RExporter ") {
        std::string fll =
                "Engine: mam21\n"
                "InputVariable: angle\n"
                "  enabled: true\n"
                "  range: -5.000 5.000\n"
                "  lock-range: false\n"
                "  term: small Bell -5.000 5.000 8.000\n"
                "  term: big Bell 5.000 5.000 8.000\n"
                "InputVariable: velocity\n"
                "  enabled: true\n"
                "  range: -5.000 5.000\n"
                "  lock-range: false\n"
                "  term: small Bell -5.000 5.000 2.000\n"
                "  term: big Bell 5.000 5.000 2.000\n"
                "OutputVariable: force\n"
                "  enabled: true\n"
                "  range: -5.000 5.000\n"
                "  lock-range: false\n"
                "  aggregation: Maximum\n"
                "  defuzzifier: Centroid 200\n"
                "  default: nan\n"
                "  lock-previous: false\n"
                "  term: negBig Bell -5.000 1.670 8.000\n"
                "  term: negSmall Bell -1.670 1.670 8.000\n"
                "  term: posSmall Bell 1.670 1.670 8.000\n"
                "  term: posBig Bell 5.000 1.670 8.000\n"
                "OutputVariable: force2\n"
                "  enabled: true\n"
                "  range: -5.000 5.000\n"
                "  lock-range: false\n"
                "  aggregation: Maximum\n"
                "  defuzzifier: Centroid 200\n"
                "  default: nan\n"
                "  lock-previous: false\n"
                "  term: negBig2 Bell -3.000 1.670 8.000\n"
                "  term: negSmall2 Bell -1.000 1.670 8.000\n"
                "  term: posSmall2 Bell 1.000 1.670 8.000\n"
                "  term: posBig2 Bell 3.000 1.670 8.000\n"
                "RuleBlock: \n"
                "  enabled: true\n"
                "  conjunction: Minimum\n"
                "  disjunction: Maximum\n"
                "  implication: Minimum\n"
                "  activation: General\n"
                "  rule: if angle is small and velocity is small then force is negBig and force2 is posBig2\n"
                "  rule: if angle is small and velocity is big then force is negSmall and force2 is posSmall2\n"
                "  rule: if angle is big and velocity is small then force is posSmall and force2 is negSmall2\n"
                "  rule: if angle is big and velocity is big then force is posBig and force2 is negBig2\n";

        Engine* engine = FllImporter().fromString(fll);
        RScriptExporter().toFile("/tmp/mam22.R", engine,
                engine->getInputVariable(0), engine->getInputVariable(1),
                1024, FldExporter::AllVariables, engine->outputVariables());
    }

}
