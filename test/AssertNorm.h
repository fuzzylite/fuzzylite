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

#ifndef FL_TEST_ASSERT_NORM_H
#define FL_TEST_ASSERT_NORM_H

#include "test/Headers.h"

namespace fuzzylite { namespace test {
    template <class T>
    struct AssertNorm {
        FL_unique_ptr<T> actual;

        explicit AssertNorm() : actual(new T) {}

        AssertNorm& repr_is(const std::string& repr) {
            FL_IUNUSED(repr);
            return *this;
        }

        AssertNorm& can_clone() {
            auto clone = actual->clone();
            CHECK(clone->className() == actual->className());
            return *this;
        }

        AssertNorm& exports_fll(const std::string& fll) {
            CHECK(actual->className() == fll);
            return *this;
        }

        AssertNorm& is_t_norm() {
            CHECK(dynamic_cast<TNorm*>(actual.get()));
            TNormFactory tnorm;
            if (not dynamic_cast<TNormFunction*>(actual.get())) {
                CAPTURE(actual->className(), tnorm.available());
                CHECK(tnorm.hasConstructor(actual->className()));
                FL_unique_ptr<TNorm> constructed(tnorm.constructObject(actual->className()));
                CHECK(constructed->className() == actual->className());
            }
            return *this;
        }

        AssertNorm& is_s_norm() {
            CHECK(dynamic_cast<SNorm*>(actual.get()));
            SNormFactory snorm;
            if (not dynamic_cast<SNormFunction*>(actual.get())) {
                CAPTURE(actual->className(), snorm.available());
                CHECK(snorm.hasConstructor(actual->className()));
                FL_unique_ptr<SNorm> constructed(snorm.constructObject(actual->className()));
                CHECK(constructed->className() == actual->className());
            }
            return *this;
        }

        AssertNorm&
        evaluates(const std::vector<std::vector<scalar>>& abz, bool commutative = true, bool associative = true) {
            for (const auto& ab_z : abz) {
                CHECK(ab_z.size() == 3);
                const auto a = ab_z.at(0);
                const auto b = ab_z.at(1);
                const auto z = ab_z.at(2);

                CAPTURE(a, b, z);
                CHECK_THAT(actual->compute(a, b), Approximates(z));
                if (commutative)
                    CHECK_THAT(actual->compute(b, a), Approximates(z));
                if (associative) {
                    const auto c = (a + b) / 2.0;
                    const auto abc = actual->compute(actual->compute(a, b), c);
                    const auto bca = actual->compute(actual->compute(b, c), a);
                    const auto cab = actual->compute(actual->compute(c, a), b);
                    CHECK_THAT(abc, Approximates(bca));
                    CHECK_THAT(bca, Approximates(cab));
                }
            }
            return *this;
        }

        AssertNorm& can_construct() {
            std::unique_ptr<Norm> object(T::constructor());
            FllExporter exporter;
            CHECK(exporter.toString(object.get()) == exporter.toString(actual.get()));
            return *this;
        }
    };

}}

#endif
