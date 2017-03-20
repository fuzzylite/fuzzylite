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
     * Tests: term/Function
     *
     * @author Juan Rada-Vilela, Ph.D.
     *
     */

    TEST_CASE("Treshold can be clone ", "[activation][threshold]") {
        Threshold* t = new Threshold("<", 1.0);
        REQUIRE(t->getComparison() == Threshold::LessThan);
        REQUIRE(Op::isEq(t->getValue(), 1.0));
        FL_DBG(FllExporter().toString(t));

        Threshold* clone = t->clone();
        REQUIRE(clone->getComparison() == Threshold::LessThan);
        REQUIRE(Op::isEq(clone->getValue(), 1.0));
        FL_DBG(FllExporter().toString(clone));
    }

    TEST_CASE("Treshold can be copy-constructed", "[activation][threshold]") {
        Threshold* t = new Threshold(">=", 1.0);
        REQUIRE(t->getComparison() == Threshold::GreaterThanOrEqualTo);
        REQUIRE(Op::isEq(t->getValue(), 1.0));
        FL_DBG(FllExporter().toString(t));

        Threshold clone(*t);
        REQUIRE(clone.getComparison() == Threshold::GreaterThanOrEqualTo);
        REQUIRE(Op::isEq(clone.getValue(), 1.0));
        FL_DBG(FllExporter().toString(&clone));
    }

    TEST_CASE("Treshold can be assigned", "[activation][threshold]") {
        Threshold* t = new Threshold(">=", 1.0);
        REQUIRE(t->getComparison() == Threshold::GreaterThanOrEqualTo);
        REQUIRE(Op::isEq(t->getValue(), 1.0));
        FL_DBG(FllExporter().toString(t));

        Threshold clone = *t;
        REQUIRE(clone.getComparison() == Threshold::GreaterThanOrEqualTo);
        REQUIRE(Op::isEq(clone.getValue(), 1.0));
        FL_DBG(FllExporter().toString(&clone));
    }

}
