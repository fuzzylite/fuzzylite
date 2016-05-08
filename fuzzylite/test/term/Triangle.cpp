/*
 Copyright © 2010-2016 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

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
            REQUIRE(fl::Op::isEq(triangle.membership(i), ramp.membership(i)));
        }
        REQUIRE(fl::Op::isEq(triangle.membership(-fl::inf), 1.0));
        REQUIRE(fl::Op::isEq(triangle.membership(fl::inf), 0.0));
    }

    TEST_CASE("triangle can be open ended with +infinity", "[term][triangle]") {
        Triangle triangle("A", 0, 1, fl::inf);
        Ramp ramp("a", 0, 1);
        for (int i = 10; i >= -2; --i) {
            FL_DBG("A(" << i << ")=" << triangle.membership(i));
            FL_DBG("a(" << i << ")=" << ramp.membership(i));
            REQUIRE(fl::Op::isEq(triangle.membership(i), ramp.membership(i)));
        }
        REQUIRE(fl::Op::isEq(triangle.membership(fl::inf), 1.0));
        REQUIRE(fl::Op::isEq(triangle.membership(-fl::inf), 0.0));
    }

}

