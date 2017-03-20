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
     * Tests: term/Discrete
     *
     * @author Juan Rada-Vilela, Ph.D.
     *
     */

    TEST_CASE("discrete finds elements using binary search", "[term][discrete]") {
        fuzzylite::setLogging(true);
        fuzzylite::setDebugging(false);
        Rectangle rectangle("rectangle", 0, 1);
        FL_unique_ptr<Discrete> discrete(Discrete::discretize(&rectangle, rectangle.getStart(), rectangle.getEnd(), 10));
        FL_LOG(discrete->toString());

        CHECK(discrete->membership(.25) == 1.0);
        CHECK(discrete->membership(0.0) == 1.0);
        CHECK(discrete->membership(-1.0) == 1.0);
        CHECK(discrete->membership(1.0) == 1.0);
        CHECK(discrete->membership(2.0) == 1.0);
        CHECK(Op::isNaN(discrete->membership(fl::nan)));
        CHECK(discrete->membership(fl::inf) == 1.0);
        CHECK(discrete->membership(-fl::inf) == 1.0);
    }

    TEST_CASE("discrete still finds elements using binary search", "[term][discrete]") {
        fuzzylite::setLogging(true);
        fuzzylite::setDebugging(false);
        Triangle triangle("triangle", 0, 1);
        FL_unique_ptr<Discrete> discrete(Discrete::discretize(&triangle, triangle.getVertexA(), triangle.getVertexC(), 100));
        FL_LOG(discrete->toString());
        for (int i = 0; i < 200; ++i) {
            scalar x = Op::scale(i, 0, 200, -1, 1);
            if (not Op::isEq(triangle.membership(x), discrete->membership(x))) {
                fuzzylite::setDebugging(true);
                CHECK(Op::isEq(triangle.membership(x), discrete->membership(x)));
                fuzzylite::setDebugging(false);
            }
        }
    }

    TEST_CASE("discrete finds all elements using binary search", "[term][discrete]") {
        fuzzylite::setLogging(true);
        fuzzylite::setDebugging(false);
        scalar min = -1.0;
        scalar max = 1.0;
        scalar range = max - min;
        scalar mean = 0.5 * (max + min);


        std::vector<Term*> terms;
        terms.push_back(new Triangle("triangle", min, mean, max));
        terms.push_back(new Trapezoid("trapezoid", min, min + .25 * range, min + .75 * range, max));
        terms.push_back(new Rectangle("rectangle", min, max));
        terms.push_back(new Bell("bell", mean, range / 4, 3.0));
        terms.push_back(new Cosine("cosine", mean, range));
        terms.push_back(new Gaussian("gaussian", mean, range / 4));
        terms.push_back(new GaussianProduct("gaussianProduct", mean, range / 4, mean, range / 4));
        terms.push_back(new PiShape("piShape", min, mean, mean, max));
        terms.push_back(new SigmoidDifference("sigmoidDifference", min + .25 * range, 20 / range, 20 / range, max - .25 * range));
        terms.push_back(new SigmoidProduct("sigmoidProduct", min + .25 * range, 20 / range, 20 / range, max - .25 * range));
        terms.push_back(new Spike("spike", mean, range));

        terms.push_back(new Binary("binary", min, max));
        terms.push_back(new Concave("concave", mean, max));
        terms.push_back(new Ramp("ramp", min, max));
        terms.push_back(new Sigmoid("sigmoid", mean, 20 / range));
        terms.push_back(new SShape("sshape", min, max));
        terms.push_back(new ZShape("zshape", min, max));

        for (std::size_t t = 0; t < terms.size(); ++t) {
            Term* term = terms.at(t);
            std::vector<Discrete::Pair> pairs;
            srand(0);
            for (int i = 0; i < 1000; ++i) {
                int randomX = std::rand();
                scalar x = Op::scale(randomX % 100, 0, 100, -1, 1);
                pairs.push_back(Discrete::Pair(x, term->membership(x)));
            }
            Discrete::sort(pairs);

            Discrete discrete("discrete", pairs);
            for (std::size_t i = 0; i < pairs.size(); ++i) {
                Discrete::Pair pair = pairs.at(i);
                scalar x = pair.first;
                if (not Op::isEq(discrete.membership(x), term->membership(x))) {
                    fuzzylite::setDebugging(true);
                    CHECK(discrete.membership(x) == term->membership(x));
                    fuzzylite::setDebugging(false);
                }
            }
            for (int i = 0 ; i < 100 ; i++) {
                scalar x = Op::scale(i, 0, 100, -1, 1);
                if (not Op::isEq(discrete.membership(x), term->membership(x))) {
                    fuzzylite::setDebugging(true);
                    CHECK(discrete.membership(x) == term->membership(x));
                    fuzzylite::setDebugging(false);
                }
            }
        }
        for (std::size_t i = 0 ; i < terms.size(); ++i){
            delete terms.at(i);
        }
    }
}
