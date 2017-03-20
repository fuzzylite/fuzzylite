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

    TEST_CASE("function parses basic arithmetic", "[term][function]") {
        Function f;
        std::string text = "3+4*2/(1-5)^2^3";
        CHECK(f.toPostfix(text) == "3 4 2 * 1 5 - 2 3 ^ ^ / +");
        CHECK(f.parse(text)->toInfix() == "3.000 ^ 2.000 ^ 5.000 - 1.000 / 2.000 * 4.000 + 3.000");
        CHECK(f.parse(text)->toPrefix() == "+ / ^ ^ 3.000 2.000 - 5.000 1.000 * 2.000 4.000 3.000");
    }

    TEST_CASE("function parses basic trigonometry", "[term][function]") {
        Function f;
        std::string text = "sin(y*x)^2/x";

        CHECK_THROWS(f.load(text));

        f.variables["y"] = 1.0;
        f.load(text);

        CHECK(f.toPostfix(text) == "y x * sin 2 ^ x /");
        CHECK(f.parse(text)->toInfix() == "x / 2.000 ^ x * y sin");
        CHECK(f.parse(text)->toPrefix() == "/ x ^ 2.000 sin * x y");
    }

    TEST_CASE("function parses propositions", "[term][function]") {
        Function f;

        std::string text = "(Temperature is High and Oxygen is Low) or "
                "(Temperature is Low and (Oxygen is Low or Oxygen is High))";

        CHECK(f.toPostfix(text) == "Temperature is High Oxygen is Low "
                "and Temperature is Low Oxygen is Low Oxygen is High or and or");
    }

    TEST_CASE("function cannot deal with negative numbers", "[term][function]") {
        Function f;
        std::string text = "-5 *4/sin(-pi/2)";

        SECTION("function throws exception") {
            CHECK_THROWS(f.parse(text)->evaluate());
        }

        f.variables["pi"] = 3.14;
        CHECK_THROWS(f.parse(text)->evaluate(&f.variables));

        text = "~5 *4/sin(~pi/2)";
        CHECK(f.parse(text)->evaluate(&f.variables) == Approx(20));

        f.load(text);

        f.variables["pi"] = 3.14;

        CHECK(f.toPostfix(text) == "5 ~ 4 * pi ~ 2 / sin /");
        CHECK(f.parse(text)->toInfix() == "2.000 / pi ~ sin / 4.000 * 5.000 ~");
        CHECK(f.parse(text)->toPrefix() == "/ sin / 2.000 ~ pi * 4.000 ~ 5.000");
    }

    TEST_CASE("Function is clonable", "[term][function]") {
        Function* f = new Function;
        std::string text = "2+2";
        f->load(text);
        CHECK(Op::isEq(f->membership(fl::nan), 4));
        Function* clone = f->clone();
        delete f;
        CHECK(Op::isEq(clone->membership(fl::nan), 4));
        delete clone;
    }

    TEST_CASE("Function is constructor copyable", "[term][function]") {
        Function* f = new Function;
        std::string text = "2+2";
        f->load(text);
        CHECK(Op::isEq(f->membership(fl::nan), 4));
        Function* clone = new Function(*f);
        delete f;
        CHECK(Op::isEq(clone->membership(fl::nan), 4));
        delete clone;
    }

    TEST_CASE("Function computes tree size correctly", "[term][function]"){
        Function f("f", "x*x+(x-x)/x+log(x)");
        f.load();
        CHECK(f.root()->treeSize() == 6);
        CHECK(f.root()->treeSize(Function::Element::Function) == 1);
        CHECK(f.root()->treeSize(Function::Element::Operator) == 5);
        FL_LOG(f.complexity().toString());
    }

}
