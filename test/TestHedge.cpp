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

#include "Headers.h"

namespace fuzzylite { namespace test {
    struct HedgeAssert {
        FL_unique_ptr<Hedge> actual;

        HedgeAssert(Hedge* actual) : actual(actual) {}

        HedgeAssert& has_name(const std::string& name, bool checkFactory = true, bool canClone = true) {
            CHECK(actual->name() == name);
            if (checkFactory)
                in_factory();
            if (canClone)
                can_clone();
            return *this;
        }

        HedgeAssert& in_factory() {
            const HedgeFactory hf;
            CHECK(hf.hasConstructor(actual->name()));
            CHECK(FL_unique_ptr<Hedge>(hf.constructObject(actual->name()))->name() == actual->name());
            return *this;
        }

        HedgeAssert& can_clone() {
            CHECK(FL_unique_ptr<Hedge>(actual->clone())->name() == actual->name());
            return *this;
        }

        HedgeAssert& evaluates(const std::vector<std::vector<double>>& az) {
            for (const auto& item : az) {
                CAPTURE(item);
                CHECK(item.size() == 2);
                const double a = item.front();
                const double z = item.back();

                CAPTURE(a, z);
                CHECK_THAT(actual->hedge(a), Approximates(z));
            }
            return *this;
        }
    };

    TEST_CASE("Hedge", "[hedge]") {
        SECTION("Any") {
            HedgeAssert(new Any()).has_name("any").evaluates({
                {-1.0, 1.0},
                {-0.5, 1.0},
                {0.00, 1.0},
                {0.25, 1.0},
                {0.50, 1.0},
                {0.75, 1.0},
                {1.00, 1.0},
                {inf, 1.0},
                {-inf, 1.0},
                {nan, 1.0},
            });
        }

        SECTION("Extremely") {
            HedgeAssert(new Extremely())
                .has_name("extremely")
                .evaluates({
                    {-1.0, 2.0},
                    {-0.5, 0.5},
                    {0.00, 0.0},
                    {0.25, 0.125},
                    {0.50, 0.5},
                    {0.75, 0.875},
                    {1.00, 1.0},
                    {inf, -inf},
                    {-inf, inf},
                    {nan, nan},
                });
        }

        SECTION("Not") {
            HedgeAssert(new Not()).has_name("not").evaluates({
                {-1.0, 2.0},
                {-0.5, 1.5},
                {0.00, 1.0},
                {0.25, 0.75},
                {0.50, 0.50},
                {0.75, 0.25},
                {1.00, 0.00},
                {inf, -inf},
                {-inf, inf},
                {nan, nan},
            });
        }

        SECTION("Seldom") {
            HedgeAssert(new Seldom())
                .has_name("seldom")
                .evaluates({
                    {-1.0, nan},
                    {-0.5, nan},
                    {0.00, 0.0},
                    {0.25, 0.3535533905932738},
                    {0.50, 0.5},
                    {0.75, 0.6464466094067263},
                    {1.00, 1.0},
                    {inf, nan},
                    {-inf, nan},
                    {nan, nan},
                });
        }

        SECTION("Somewhat") {
            HedgeAssert(new Somewhat())
                .has_name("somewhat")
                .evaluates({
                    {-1.0, nan},
                    {-0.5, nan},
                    {0.00, 0.0},
                    {0.25, 0.5},
                    {0.50, 0.7071067811865476},
                    {0.75, 0.8660254037844386},
                    {1.00, 1.0},
                    {inf, inf},
                    {-inf, nan},
                    {nan, nan},
                });
        }

        SECTION("Very") {
            HedgeAssert(new Very())
                .has_name("very")
                .evaluates({
                    {-1.0, 1.0},
                    {-0.5, 0.25},
                    {0.00, 0.0},
                    {0.25, 0.0625},
                    {0.50, 0.25},
                    {0.75, 0.5625},
                    {1.00, 1.0},
                    {inf, inf},
                    {-inf, inf},
                    {nan, nan},
                });
        }

        SECTION("Function") {
            HedgeAssert(new HedgeFunction("x^2", "my_hedge"))
                .has_name("my_hedge", false)
                .evaluates({
                    {-1.0, 1.0},
                    {-0.5, 0.25},
                    {0.00, 0.0},
                    {0.25, 0.0625},
                    {0.50, 0.25},
                    {0.75, 0.5625},
                    {1.00, 1.0},
                    {inf, inf},
                    {-inf, inf},
                    {nan, nan},
                });

            FL_unique_ptr<HedgeFunction> f(dynamic_cast<HedgeFunction*>(HedgeFunction::constructor()));
            CHECK(f->getFormula() == "");
            f->setFormula("x^3");
            CHECK(f->getFormula() == "x^3");
            CHECK(f->function().evaluate({{"x", 3}}) == 27);
        }
    }

}}
