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
     * Tests: term/Trapezoid
     *
     * @author Juan Rada-Vilela, Ph.D.
     *
     */

    TEST_CASE("trapezoid can be open ended with -infinity", "[term][trapezoid]") {
        Trapezoid trapezoid("A", -fl::inf, 0, 1, 2);
        Ramp ramp("a", 2, 1);
        //(-inf, inf)
        for (scalar i = -10.0; Op::isLE(i, 10.0); i += .2) {
            FL_DBG("A(" << i << ")=" << trapezoid.membership(i));
            FL_DBG("a(" << i << ")=" << ramp.membership(i));
            REQUIRE(Op::isEq(trapezoid.membership(i), ramp.membership(i)));
        }
        REQUIRE(Op::isEq(trapezoid.membership(-fl::inf), 1.0));
        REQUIRE(Op::isEq(trapezoid.membership(fl::inf), 0.0));
    }

    TEST_CASE("trapezoid can be open ended with +infinity", "[term][trapezoid]") {
        Trapezoid trapezoid("A", 0, 1, 2, fl::inf);
        Ramp ramp("a", 0, 1);
        //(-inf, inf)
        for (scalar i = -10.0; Op::isLE(i, 10.0); i += .2) {
            REQUIRE(Op::isEq(trapezoid.membership(i), ramp.membership(i)));
        }
        REQUIRE(Op::isEq(trapezoid.membership(fl::inf), 1.0));
        REQUIRE(Op::isEq(trapezoid.membership(-fl::inf), 0.0));
    }

}
