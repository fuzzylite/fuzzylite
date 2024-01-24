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
#include <catch2/catch.hpp>

#include "fl/Headers.h"

namespace fuzzylite {
    struct TermAssert {
        FL_unique_ptr<Term> actual;

        TermAssert(Term* actual) : actual(actual) {
            fuzzylite::fuzzylite::setMachEps(1e-3);
        }

        TermAssert& fllExportsTo(const std::string& obtained) {
            CHECK(this->actual->toString() == obtained);
            return *this;
        }

        TermAssert& isMonotonic(bool monotonic = true) {
            CHECK(this->actual->isMonotonic() == monotonic);
            return *this;
        }

        TermAssert& isNotMonotonic() {
            return this->isMonotonic(false);
        }

        TermAssert& takesParameters(int requiredParameters, bool height = true) {
            const std::string& fll = this->actual->toString();
            this->actual->configure("");

            return *this;
        }

        TermAssert& configuredAs(const std::string& parameters) {
            this->actual->configure(parameters);
            return *this;
        }

        TermAssert& hasMemberships(const std::map<fl::scalar, fl::scalar>& values, scalar height=1.0) {
            for (const auto& value : values) {
                const auto& x = value.first;
                const auto& expected_y = height * value.second;

                const auto& obtained_y = this->actual->membership(x);
                CAPTURE(x);
                CHECK_THAT(obtained_y, Catch::Matchers::WithinAbs(expected_y, fuzzylite::macheps()));
            }
            return *this;
        }
    };

    TEST_CASE("Term configured with empty string throws exception", "[term]") {
        TermAssert(new Bell("bell")).takesParameters(3);
    }

    TEST_CASE("Bell", "[term][bell]") {
        TermAssert(new Bell("bell"))
            .fllExportsTo("term: bell Bell nan nan nan")
            .isNotMonotonic()
            .configuredAs("0 0.25 3.0")
            .fllExportsTo("term: bell Bell 0.000 0.250 3.000")
            .hasMemberships({
                {-1.0, 0.0},
                {-0.5, 0.015384615384615385},
                {-0.4, 0.05625177755617076},
                {-0.25, 0.5},
                {-0.1, 0.9959207087768499},
                {0.0, 1.0},
                {0.1, 0.9959207087768499},
                {0.25, 0.5},
                {0.4, 0.05625177755617076},
                {0.5, 0.015384615384615385},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

}  // namespace fl
