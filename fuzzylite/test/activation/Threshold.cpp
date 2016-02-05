/*
 Copyright © 2010-2015 FuzzyLite Limited.
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
     * Tests: term/Function
     * 
     * @author Juan Rada-Vilela, Ph.D.
     * 
     */

    TEST_CASE("Treshold can be clone ", "[activation][threshold]") {
        Threshold* t = new Threshold("<", 1.0);
        REQUIRE(t->getComparison() == Threshold::LessThan);
        REQUIRE(fl::Op::isEq(t->getThreshold(), 1.0));
        FL_DBG(FllExporter().toString(t));

        Threshold* clone = t->clone();
        REQUIRE(clone->getComparison() == Threshold::LessThan);
        REQUIRE(fl::Op::isEq(clone->getThreshold(), 1.0));
        FL_DBG(FllExporter().toString(clone));
    }

    TEST_CASE("Treshold can be copy-constructed", "[activation][threshold]") {
        Threshold* t = new Threshold(">=", 1.0);
        REQUIRE(t->getComparison() == Threshold::GreaterThanOrEqualTo);
        REQUIRE(fl::Op::isEq(t->getThreshold(), 1.0));
        FL_DBG(FllExporter().toString(t));

        Threshold clone(*t);
        REQUIRE(clone.getComparison() == Threshold::GreaterThanOrEqualTo);
        REQUIRE(fl::Op::isEq(clone.getThreshold(), 1.0));
        FL_DBG(FllExporter().toString(&clone));
    }

    TEST_CASE("Treshold can be assigned", "[activation][threshold]") {
        Threshold* t = new Threshold(">=", 1.0);
        REQUIRE(t->getComparison() == Threshold::GreaterThanOrEqualTo);
        REQUIRE(fl::Op::isEq(t->getThreshold(), 1.0));
        FL_DBG(FllExporter().toString(t));

        Threshold clone = *t;
        REQUIRE(clone.getComparison() == Threshold::GreaterThanOrEqualTo);
        REQUIRE(fl::Op::isEq(clone.getThreshold(), 1.0));
        FL_DBG(FllExporter().toString(&clone));
    }

}
