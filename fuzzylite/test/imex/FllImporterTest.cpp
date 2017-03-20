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

    TEST_CASE("Template of FuzzyLite Language works", "[imex]") {
        std::string fllTemplate;
#ifdef FL_CPP98
        //ignore
#else
        fllTemplate = R""(
#Template: FuzzyLite Language (FLL)
#Engine: string
#InputVariable: identifier
#  enabled: boolean
#  range: scalar scalar
#  term: identifier Term [parameters]
#OutputVariable: identifier
#  enabled: boolean
#  range: scalar scalar
#  aggregation: SNorm|none
#  defuzzifier: [Defuzzifier [parameter]]|none
#  default: scalar
#  lock-previous: boolean
#  lock-range: boolean
#  term: identifier Term [parameters]
#RuleBlock: string
#  enabled: boolean
#  conjunction: TNorm|none
#  disjunction: SNorm|none
#  implication: TNorm|none
#  rule: if antecedent then consequent with weight
)"";
#endif
        FL_unique_ptr<Engine> engine(FllImporter().fromString(fllTemplate));
        Engine empty;
        CHECK(FllExporter().toString(engine.get()) == FllExporter().toString(&empty));
    }

}
