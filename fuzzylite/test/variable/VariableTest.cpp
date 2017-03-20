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

#include <algorithm>    // std::random_shuffle

namespace fl {

    /**
     * Tests: variable/Variable
     *
     * @author Juan Rada-Vilela, Ph.D.
     *
     */

    TEST_CASE("variable of Constant terms is sorted", "[variable][variable]") {
        Variable variable("Variable", -10, 10);
        for (int i = 0; i <= 20; ++i) {
            variable.addTerm(new Constant(Op::str(i), i - 10));
        }
        std::random_shuffle(variable.terms().begin(), variable.terms().end());
        FL_DBG(variable.toString());
        REQUIRE(variable.numberOfTerms() == 21);
        variable.sort();
        REQUIRE(variable.numberOfTerms() == 21);
        int value = -10;
        for (std::size_t i = 0; i < variable.terms().size(); ++i) {
            Constant* term = dynamic_cast<Constant*> (variable.terms().at(i));
            REQUIRE(term);
            REQUIRE(term->getValue() == float(value));
            ++value;
        }
        FL_DBG(variable.toString());
    }

    TEST_CASE("variable of Triangle terms is sorted", "[variable][variable]") {
        Variable variable("Variable", -30, 30);
        for (int i = 0; i <= 20; ++i) {
            variable.addTerm(new Triangle(Op::str(i), i - 1, i, i + 1));
        }
        std::random_shuffle(variable.terms().begin(), variable.terms().end());
        FL_DBG(variable.toString());
        REQUIRE(variable.numberOfTerms() == 21);
        variable.sort();
        FL_DBG("Shuffled:");
        FL_DBG(variable.toString());
        REQUIRE(variable.numberOfTerms() == 21);
        int value = 0;
        for (std::size_t i = 0; i < variable.terms().size(); ++i) {
            Triangle* term = dynamic_cast<Triangle*> (variable.terms().at(i));
            REQUIRE(term);
            REQUIRE(term->getName() == Op::str(value < 0 ? -1 * value : value));
            ++value;
        }
        FL_DBG(variable.toString());
    }

}

