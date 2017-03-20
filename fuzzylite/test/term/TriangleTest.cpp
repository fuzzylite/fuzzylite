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

    /**
     * Tests: term/Triangle
     *
     * @author Juan Rada-Vilela, Ph.D.
     *
     */

    TEST_CASE("triangle can be open ended with -infinity", "[term][triangle]") {
        Triangle triangle("A", -fl::inf, 0, 1);
        Ramp ramp("a", 1, 0);
        for (int i = -10; i < 2; ++i) {
            FL_DBG("A(" << i << ")=" << triangle.membership(i));
            FL_DBG("a(" << i << ")=" << ramp.membership(i));
            REQUIRE(Op::isEq(triangle.membership(i), ramp.membership(i)));
        }
        REQUIRE(Op::isEq(triangle.membership(-fl::inf), 1.0));
        REQUIRE(Op::isEq(triangle.membership(fl::inf), 0.0));
    }

    TEST_CASE("triangle can be open ended with +infinity", "[term][triangle]") {
        Triangle triangle("A", 0, 1, fl::inf);
        Ramp ramp("a", 0, 1);
        for (int i = 10; i >= -2; --i) {
            FL_DBG("A(" << i << ")=" << triangle.membership(i));
            FL_DBG("a(" << i << ")=" << ramp.membership(i));
            REQUIRE(Op::isEq(triangle.membership(i), ramp.membership(i)));
        }
        REQUIRE(Op::isEq(triangle.membership(fl::inf), 1.0));
        REQUIRE(Op::isEq(triangle.membership(-fl::inf), 0.0));
    }

    TEST_CASE("triangle defuzzification is correct", "[term][triangle]") {
        Triangle a("A", 0, 1, 2);
        Triangle b("B", 1, 2, 3);
        AlgebraicProduct times;
        Aggregated x("X", 0, 3, new AlgebraicSum);
        x.addTerm(&a, 1, &times);
        x.addTerm(&b, 1, &times);
        Centroid c(101);
        FL_LOG(c.defuzzify(&x, 0, 3));

        Triangle t("T", 0, 0, 1);
        FL_LOG(c.defuzzify(&t, 0, 1));
    }
}
