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

#ifndef FL_TEST_ASSERT_HEDGE_H
#define FL_TEST_ASSERT_HEDGE_H

#include "test/Headers.h"

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
}}

#endif
