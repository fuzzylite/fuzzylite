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
     * Tests: term/Aggregated
     *
     * @author Juan Rada-Vilela, Ph.D.
     *
     */

    TEST_CASE("highest term in aggregated", "[term][aggregated]") {

        FL_unique_ptr<Term> dark(new Triangle("DARK", 0.000, 0.250, 0.500));
        FL_unique_ptr<Term> medium(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
        FL_unique_ptr<Term> bright(new Triangle("BRIGHT", 0.500, 0.750, 1.000));

        Aggregated aggregated;
        aggregated.addTerm(dark.get(), 0.5, fl::null);
        aggregated.addTerm(medium.get(), 0.1, fl::null);
        aggregated.addTerm(bright.get(), 0.6, fl::null);

        REQUIRE(aggregated.highestActivatedTerm()->getTerm() == bright.get());

        aggregated.terms().at(1).setDegree(0.7);
        REQUIRE(aggregated.highestActivatedTerm()->getTerm() == medium.get());

        aggregated.terms().front().setDegree(0.9);
        REQUIRE(aggregated.highestActivatedTerm()->getTerm() == dark.get());

        aggregated.clear();
        REQUIRE(aggregated.highestActivatedTerm() == fl::null);
    }

}
