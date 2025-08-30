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

#include "test/AssertNorm.h"
#include "test/Headers.h"

namespace fuzzylite { namespace test {

    TEST_CASE("TNorm", "[norm][t]") {
        SECTION("AlgebraicProduct") {
            NormAssert<AlgebraicProduct>()
                .is_t_norm()
                .repr_is("fl.AlgebraicProduct()")
                .exports_fll("AlgebraicProduct")
                .can_clone()
                .can_construct()
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
            NormAssert<BoundedDifference>()
                .is_t_norm()
                .repr_is("fl.BoundedDifference()")
                .exports_fll("BoundedDifference")
                .can_clone()
                .can_construct()
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
            NormAssert<DrasticProduct>()
                .is_t_norm()
                .repr_is("fl.DrasticProduct()")
                .exports_fll("DrasticProduct")
                .can_clone()
                .can_construct()
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
            NormAssert<EinsteinProduct>()
                .is_t_norm()
                .repr_is("fl.EinsteinProduct()")
                .exports_fll("EinsteinProduct")
                .can_clone()
                .can_construct()
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
            NormAssert<HamacherProduct>()
                .is_t_norm()
                .repr_is("fl.HamacherProduct()")
                .exports_fll("HamacherProduct")
                .can_clone()
                .can_construct()
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
            NormAssert<Minimum>()
                .is_t_norm()
                .repr_is("fl.Minimum()")
                .exports_fll("Minimum")
                .can_clone()
                .can_construct()
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
            NormAssert<NilpotentMinimum>()
                .is_t_norm()
                .repr_is("fl.NilpotentMinimum()")
                .exports_fll("NilpotentMinimum")
                .can_clone()
                .can_construct()
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
            auto assert = NormAssert<TNormFunction>();
            assert.actual->setFormula("a * b");
            assert.exports_fll("TNormFunction")
                .is_t_norm()
                .repr_is("fl.NormFunction(fl.Function('AlgebraicProduct', 'a * b'))")
                .can_clone()  // TODO: does not check formula
                .can_construct()
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
            NormAssert<AlgebraicSum>()
                .is_s_norm()
                .repr_is("fl.AlgebraicSum()")
                .exports_fll("AlgebraicSum")
                .can_clone()
                .can_construct()
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
            NormAssert<BoundedSum>()
                .is_s_norm()
                .repr_is("fl.BoundedSum()")
                .exports_fll("BoundedSum")
                .can_clone()
                .can_construct()
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
            NormAssert<DrasticSum>()
                .is_s_norm()
                .repr_is("fl.DrasticSum()")
                .exports_fll("DrasticSum")
                .can_clone()
                .can_construct()
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
            NormAssert<EinsteinSum>()
                .is_s_norm()
                .repr_is("fl.EinsteinSum()")
                .exports_fll("EinsteinSum")
                .can_clone()
                .can_construct()
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
            NormAssert<HamacherSum>()
                .is_s_norm()
                .repr_is("fl.HamacherSum()")
                .exports_fll("HamacherSum")
                .can_clone()
                .can_construct()
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
            NormAssert<Maximum>()
                .is_s_norm()
                .repr_is("fl.Maximum()")
                .exports_fll("Maximum")
                .can_clone()
                .can_construct()
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
            NormAssert<NilpotentMaximum>()
                .is_s_norm()
                .repr_is("fl.NilpotentMaximum()")
                .exports_fll("NilpotentMaximum")
                .can_clone()
                .can_construct()
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
            NormAssert<NormalizedSum>()
                .is_s_norm()
                .repr_is("fl.NormalizedSum()")
                .exports_fll("NormalizedSum")
                .can_clone()
                .can_construct()
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
        SECTION("UnboundedSum") {
            NormAssert<UnboundedSum>()
                .is_s_norm()
                .repr_is("fl.UnboundedSum()")
                .exports_fll("UnboundedSum")
                .can_clone()
                .can_construct()
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
            auto assert = NormAssert<SNormFunction>();
            assert.actual->setFormula("a + b - (a * b)");
            assert.exports_fll("SNormFunction")
                .is_s_norm()
                .repr_is("fl.NormFunction(fl.Function('AlgebraicSum', 'a + b - (a * b)'))")
                .can_clone()  // TODO: does not check formula
                .can_construct()
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

}}
