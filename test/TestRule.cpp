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

#include "Headers.h"

namespace fuzzylite { namespace test {

    TEST_CASE("Rule: Expression: Proposition", "[rule][expression][proposition]") {
        SECTION("Empty proposition") {
            CHECK(Proposition().toString() == "");
        }
        SECTION("Proposition with Variable") {
            Variable variable("variable");
            Constant term("term");

            Proposition proposition(&variable, {new Very}, &term);
            CHECK(proposition.toString() == "variable is very term");
        }
    }

    TEST_CASE("Rule: Expression: Operators", "[rule][expression][operator]") {
        SECTION("Empty operator") {
            CHECK(fl::Operator().name.empty());
        }
        SECTION("Operator without operands") {
            CHECK(fl::Operator("OR").toString() == "OR");
        }
        SECTION("Operator with operands") {
            Variable variable("variable");
            Constant term("term");

            Proposition proposition(&variable, {}, &term);
            CHECK(fl::Operator("AND", &proposition, &proposition).toString() == "AND");
        }
    }

    std::string simple_dimmer() {
        return std::string("Engine: SimpleDimmer"
                           "InputVariable: Ambient"
                           "  enabled: true"
                           "  range: 0.000 1.000"
                           "  lock-range: false"
                           "  term: DARK Triangle 0.000 0.250 0.500"
                           "  term: MEDIUM Triangle 0.250 0.500 0.750"
                           "  term: BRIGHT Triangle 0.500 0.750 1.000"
                           "OutputVariable: Power"
                           "  enabled: true"
                           "  range: 0.000 1.000"
                           "  lock-range: false"
                           "  aggregation: Maximum"
                           "  defuzzifier: Centroid 200"
                           "  default: nan"
                           "  lock-previous: false"
                           "  term: LOW Triangle 0.000 0.250 0.500"
                           "  term: MEDIUM Triangle 0.250 0.500 0.750"
                           "  term: HIGH Triangle 0.500 0.750 1.000");
    }

    struct AssertAntecedent {};

}}
