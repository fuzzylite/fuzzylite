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

namespace fl {

    TEST_CASE("Exports same number of values in scopes", "[imex]") {
        FL_unique_ptr<Engine> engine(Console::mamdani());
        engine->addInputVariable(new InputVariable("Dummy2", 0, 1));
        engine->addInputVariable(new InputVariable("Dummy3", 0, 1));
        engine->addInputVariable(new InputVariable("Dummy4", 0, 1));

        FldExporter exporter("\t");
        exporter.setExportHeader(false);

        int valuesEachVariable = 3;
        int expectedValues = (int) std::pow(valuesEachVariable, 1.0 * engine->numberOfInputVariables());

        std::string eachVariable = exporter.toString(engine.get(), valuesEachVariable, FldExporter::EachVariable);
        //        FL_LOG("eachVariable:\n" << eachVariable);
        std::vector<std::string> linesByVariable = Op::split(eachVariable, "\n");
        CHECK(int(linesByVariable.size()) == expectedValues);

        std::string allVariables = exporter.toString(engine.get(), expectedValues, FldExporter::AllVariables);
        std::vector<std::string> linesAllVariables = Op::split(allVariables, "\n");
        //        FL_LOG("allVariables:\n" << allVariables);
        CHECK(int(linesAllVariables.size()) == expectedValues);
    }

}
