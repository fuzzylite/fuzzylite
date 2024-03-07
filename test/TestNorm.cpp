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
#include <catch2/catch.hpp>

#include "Headers.h"
#include "fl/Headers.h"

namespace fuzzylite {
    struct NormAssert {
        FL_unique_ptr<Norm> actual;

        NormAssert(Norm* actual) : actual(actual) {}

        NormAssert& repr_is(const std::string& repr) {
            FL_IUNUSED(repr);
            return *this;
        }

        NormAssert& can_clone() {
            auto clone = actual->clone();
            CHECK(clone->className() == actual->className());
            return *this;
        }

        NormAssert& exports_fll(const std::string& fll) {
            CHECK(actual->className() == fll);
            can_clone();
            return *this;
        }

        NormAssert& is_t_norm() {
            CHECK(dynamic_cast<TNorm*>(actual.get()));
            TNormFactory tnorm;
            if (not dynamic_cast<TNormFunction*>(actual.get())) {
                CAPTURE(actual->className(), tnorm.available());
                CHECK(tnorm.hasConstructor(actual->className()));
                CHECK(
                    FL_unique_ptr<TNorm>(tnorm.constructObject(actual->className()))->className() == actual->className()
                );
            }
            return *this;
        }

        NormAssert& is_s_norm() {
            CHECK(dynamic_cast<SNorm*>(actual.get()));
            SNormFactory snorm;
            if (not dynamic_cast<SNormFunction*>(actual.get())) {
                CAPTURE(actual->className(), snorm.available());
                CHECK(snorm.hasConstructor(actual->className()));
                CHECK(
                    FL_unique_ptr<SNorm>(snorm.constructObject(actual->className()))->className() == actual->className()
                );
            }
            return *this;
        }

        NormAssert&
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
    };

    TEST_CASE("TNorm", "[norm][t]") {
        SECTION("AlgebraicProduct") {
            NormAssert(new AlgebraicProduct)
                .is_t_norm()
                .repr_is("fl.AlgebraicProduct()")
                .exports_fll("AlgebraicProduct")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.00},
                    {0.00, 0.50, 0.00},
                    {0.00, 0.75, 0.00},
                    {0.00, 1.00, 0.00},
                    {0.50, 0.25, 0.125},
                    {0.50, 0.50, 0.250},
                    {0.50, 0.75, 0.375},
                    {1.00, 0.00, 0.00},
                    {1.00, 0.25, 0.25},
                    {1.00, 0.50, 0.50},
                    {1.00, 0.75, 0.75},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, inf},
                    {inf, -inf, -inf},
                    {-inf, -inf, inf},
                });
        }

        SECTION("BoundedDifference") {
            NormAssert(new BoundedDifference)
                .is_t_norm()
                .repr_is("fl.BoundedDifference()")
                .exports_fll("BoundedDifference")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.00},
                    {0.00, 0.50, 0.00},
                    {0.00, 0.75, 0.00},
                    {0.00, 1.00, 0.00},
                    {0.50, 0.25, 0.0},
                    {0.50, 0.50, 0.0},
                    {0.50, 0.75, 0.25},
                    {1.00, 0.00, 0.00},
                    {1.00, 0.25, 0.25},
                    {1.00, 0.50, 0.50},
                    {1.00, 0.75, 0.75},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, inf},
                    {inf, -inf, nan},
                    {-inf, -inf, 0},
                });
        }

        SECTION("DrasticProduct") {
            NormAssert(new DrasticProduct)
                .is_t_norm()
                .repr_is("fl.DrasticProduct()")
                .exports_fll("DrasticProduct")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.00},
                    {0.00, 0.50, 0.00},
                    {0.00, 0.75, 0.00},
                    {0.00, 1.00, 0.00},
                    {0.50, 0.25, 0.00},
                    {0.50, 0.50, 0.00},
                    {0.50, 0.75, 0.00},
                    {1.00, 0.00, 0.00},
                    {1.00, 0.25, 0.25},
                    {1.00, 0.50, 0.50},
                    {1.00, 0.75, 0.75},
                    {1.00, 1.00, 1.00},
                    {nan, nan, 0.0},
                    {inf, inf, 0.0},
                    {inf, -inf, 0.0},
                    {-inf, -inf, 0.0},
                });
        }

        SECTION("EinsteinProduct") {
            NormAssert(new EinsteinProduct())
                .is_t_norm()
                .repr_is("fl.EinsteinProduct()")
                .exports_fll("EinsteinProduct")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.00},
                    {0.00, 0.50, 0.00},
                    {0.00, 0.75, 0.00},
                    {0.00, 1.00, 0.00},
                    {0.50, 0.25, 0.09090909090909091},
                    {0.50, 0.50, 0.20},
                    {0.50, 0.75, 0.3333333333333333},
                    {1.00, 0.00, 0.00},
                    {1.00, 0.25, 0.25},
                    {1.00, 0.50, 0.50},
                    {1.00, 0.75, 0.75},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, nan},
                    {inf, -inf, nan},
                    {-inf, -inf, nan},
                });
        }

        SECTION("HamacherProduct") {
            NormAssert(new HamacherProduct())
                .is_t_norm()
                .repr_is("fl.HamacherProduct()")
                .exports_fll("HamacherProduct")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.00},
                    {0.00, 0.50, 0.00},
                    {0.00, 0.75, 0.00},
                    {0.00, 1.00, 0.00},
                    {0.50, 0.25, 0.2},
                    {0.50, 0.50, 0.3333333333333333},
                    {0.50, 0.75, 0.42857142857142855},
                    {1.00, 0.00, 0.00},
                    {1.00, 0.25, 0.25},
                    {1.00, 0.50, 0.50},
                    {1.00, 0.75, 0.75},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, nan},
                    {inf, -inf, nan},
                    {-inf, -inf, nan},
                });
        }

        SECTION("Minimum") {
            NormAssert(new Minimum())
                .is_t_norm()
                .repr_is("fl.Minimum()")
                .exports_fll("Minimum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.00},
                    {0.00, 0.50, 0.00},
                    {0.00, 0.75, 0.00},
                    {0.00, 1.00, 0.00},
                    {0.50, 0.25, 0.25},
                    {0.50, 0.50, 0.50},
                    {0.50, 0.75, 0.50},
                    {1.00, 0.00, 0.00},
                    {1.00, 0.25, 0.25},
                    {1.00, 0.50, 0.50},
                    {1.00, 0.75, 0.75},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, inf},
                    {inf, -inf, -inf},
                    {-inf, -inf, -inf},
                });
        }

        SECTION("NilpotentMinimum") {
            NormAssert(new NilpotentMinimum())
                .is_t_norm()
                .repr_is("fl.NilpotentMinimum()")
                .exports_fll("NilpotentMinimum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.00},
                    {0.00, 0.50, 0.00},
                    {0.00, 0.75, 0.00},
                    {0.00, 1.00, 0.00},
                    {0.50, 0.25, 0.0},
                    {0.50, 0.50, 0.00},
                    {0.50, 0.75, 0.50},
                    {1.00, 0.00, 0.00},
                    {1.00, 0.25, 0.25},
                    {1.00, 0.50, 0.50},
                    {1.00, 0.75, 0.75},
                    {1.00, 1.00, 1.00},
                    {nan, nan, 0.0},
                    {inf, inf, inf},
                    {inf, -inf, 0.0},
                    {-inf, -inf, 0.0},
                });
        }

        SECTION("TNormFunction") {
            NormAssert(new TNormFunction("a * b"))
                .exports_fll("TNormFunction")
                .is_t_norm()
                .repr_is("fl.NormFunction(fl.Function('AlgebraicProduct', 'a * b'))")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.00},
                    {0.00, 0.50, 0.00},
                    {0.00, 0.75, 0.00},
                    {0.00, 1.00, 0.00},
                    {0.50, 0.25, 0.125},
                    {0.50, 0.50, 0.250},
                    {0.50, 0.75, 0.375},
                    {1.00, 0.00, 0.00},
                    {1.00, 0.25, 0.25},
                    {1.00, 0.50, 0.50},
                    {1.00, 0.75, 0.75},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, inf},
                    {inf, -inf, -inf},
                    {-inf, -inf, inf},
                });
        }
    }

    TEST_CASE("SNorm", "[norm][s]") {
        SECTION("AlgebraicSum") {
            NormAssert(new AlgebraicSum())
                .is_s_norm()
                .repr_is("fl.AlgebraicSum()")
                .exports_fll("AlgebraicSum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.625},
                    {0.50, 0.50, 0.75},
                    {0.50, 0.75, 0.875},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, nan},
                    {inf, -inf, nan},
                    {-inf, -inf, -inf},
                });

            auto f = TNormFunction("a*b");
            CHECK(f.getFormula() == "a*b");
            f.setFormula("(a*b)/2");
            CHECK(f.getFormula() == "(a*b)/2");
            CHECK(f.function().isLoaded());
            const auto constructor
                = FL_unique_ptr<TNormFunction>(dynamic_cast<TNormFunction*>(TNormFunction::constructor()));
            CHECK(not constructor->function().isLoaded());
        }

        SECTION("BoundedSum") {
            NormAssert(new BoundedSum())
                .is_s_norm()
                .repr_is("fl.BoundedSum()")
                .exports_fll("BoundedSum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.75},
                    {0.50, 0.50, 1.00},
                    {0.50, 0.75, 1.00},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, 1.0},
                    {inf, -inf, nan},
                    {-inf, -inf, -inf},
                });
        }
        SECTION("DrasticSum") {
            NormAssert(new DrasticSum())
                .is_s_norm()
                .repr_is("fl.DrasticSum()")
                .exports_fll("DrasticSum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 1.00},
                    {0.50, 0.50, 1.00},
                    {0.50, 0.75, 1.00},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, 1.0},
                    {inf, inf, 1.0},
                    {inf, -inf, 1.0},
                    {-inf, -inf, 1.0},
                });
        }

        SECTION("EinsteinSum") {
            NormAssert(new EinsteinSum())
                .is_s_norm()
                .repr_is("fl.EinsteinSum()")
                .exports_fll("EinsteinSum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.6666666666666666},
                    {0.50, 0.50, 0.80},
                    {0.50, 0.75, 0.9090909090909091},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, nan},
                    {inf, -inf, nan},
                    {-inf, -inf, nan},
                });
        }
        SECTION("HamacherSum") {
            NormAssert(new HamacherSum())
                .is_s_norm()
                .repr_is("fl.HamacherSum()")
                .exports_fll("HamacherSum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.5714285714285714},
                    {0.50, 0.50, 0.6666666666666666},
                    {0.50, 0.75, 0.80},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, nan},
                    {inf, -inf, nan},
                    {-inf, -inf, nan},
                });
        }

        SECTION("Maximum") {
            NormAssert(new Maximum())
                .is_s_norm()
                .repr_is("fl.Maximum()")
                .exports_fll("Maximum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.50},
                    {0.50, 0.50, 0.50},
                    {0.50, 0.75, 0.75},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, inf},
                    {inf, -inf, inf},
                    {-inf, -inf, -inf},
                });
        }
        SECTION("NilpotentMaximum") {
            NormAssert(new NilpotentMaximum())
                .is_s_norm()
                .repr_is("fl.NilpotentMaximum()")
                .exports_fll("NilpotentMaximum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.50},
                    {0.50, 0.50, 1.00},
                    {0.50, 0.75, 1.00},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, 1.0},
                    {inf, inf, 1.0},
                    {inf, -inf, 1.0},
                    {-inf, -inf, -inf},
                });
        }
        SECTION("NormalizedSum") {
            NormAssert(new NormalizedSum())
                .is_s_norm()
                .repr_is("fl.NormalizedSum()")
                .exports_fll("NormalizedSum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.75},
                    {0.50, 0.50, 1.00},
                    {0.50, 0.75, 1.00},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, nan},
                    {inf, -inf, nan},
                    {-inf, -inf, -inf},
                });
        }
        SECTION("") {
            NormAssert(new UnboundedSum())
                .is_s_norm()
                .repr_is("fl.UnboundedSum()")
                .exports_fll("UnboundedSum")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.75},
                    {0.50, 0.50, 1.00},
                    {0.50, 0.75, 1.25},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.25},
                    {1.00, 0.50, 1.50},
                    {1.00, 0.75, 1.75},
                    {1.00, 1.00, 2.00},
                    {nan, nan, nan},
                    {inf, inf, inf},
                    {inf, -inf, nan},
                    {-inf, -inf, -inf},
                });
        }
        SECTION("SNormFunction") {
            NormAssert(new SNormFunction("a + b - (a * b)"))
                .exports_fll("SNormFunction")
                .is_s_norm()
                .repr_is("fl.NormFunction(fl.Function('AlgebraicSum', 'a + b - (a * b)'))")
                .evaluates({
                    {0.00, 0.00, 0.00},
                    {0.00, 0.25, 0.25},
                    {0.00, 0.50, 0.50},
                    {0.00, 0.75, 0.75},
                    {0.00, 1.00, 1.00},
                    {0.50, 0.25, 0.625},
                    {0.50, 0.50, 0.75},
                    {0.50, 0.75, 0.875},
                    {1.00, 0.00, 1.00},
                    {1.00, 0.25, 1.00},
                    {1.00, 0.50, 1.00},
                    {1.00, 0.75, 1.00},
                    {1.00, 1.00, 1.00},
                    {nan, nan, nan},
                    {inf, inf, nan},
                    {inf, -inf, nan},
                    {-inf, -inf, -inf},
                });
            auto f = SNormFunction("a+b");
            CHECK(f.getFormula() == "a+b");
            f.setFormula("(a+b)/2");
            CHECK(f.getFormula() == "(a+b)/2");
            CHECK(f.function().isLoaded());
            const auto constructor
                = FL_unique_ptr<SNormFunction>(dynamic_cast<SNormFunction*>(SNormFunction::constructor()));
            CHECK(not constructor->function().isLoaded());
        }
    }

}
