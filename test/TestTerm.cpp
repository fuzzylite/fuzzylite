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

#include "fl/Headers.h"

namespace fuzzylite {
    struct TermAssert {
        FL_unique_ptr<Term> actual;

        TermAssert(Term* actual) : actual(actual) {
            // TODO: Remove once changed
            fuzzylite::fuzzylite::setMachEps(1e-3);
        }

        TermAssert& exports_fll(const std::string& obtained, bool checkHeight = true) {
            CHECK(this->actual->toString() == obtained);
            if (checkHeight) {
                if (this->actual->getHeight() == 1.0) {
                    this->actual->setHeight(0.5);
                    CHECK(this->actual->toString() == obtained + " 0.500");
                    this->actual->setHeight(1.0);
                }
            }
            return *this;
        }

        TermAssert& repr_is(const std::string& obtained) {
            // TODO
            FL_IUNUSED(obtained);
            return *this;
        }

        TermAssert& is_monotonic(bool monotonic = true) {
            CHECK(this->actual->isMonotonic() == monotonic);
            return *this;
        }

        TermAssert& is_not_monotonic() {
            return this->is_monotonic(false);
        }

        TermAssert& with_height(scalar height) {
            this->actual->setHeight(height);
            return *this;
        }

        TermAssert& takes_parameters(int requiredParameters, bool height = true) {
            const fl::TermFactory* termFactory = fl::FactoryManager::instance()->term();
            FL_unique_ptr<Term> term(termFactory->constructObject(this->actual->className()));
            const std::string& expectedException = "[configuration error] term <" + this->actual->className() + ">"
                                                   + " requires <" + std::to_string(requiredParameters)
                                                   + "> parameters";

            CAPTURE(requiredParameters);
            SECTION("underparameterized") {
                // Can't be configured with fewer parameters than required
                std::vector<std::string> list;
                std::string parameters;
                for (int parameter = 0; parameter + 1 < requiredParameters; ++parameter) {
                    list.push_back("nan");
                    CAPTURE(parameters = fl::Op::join(list, " "));
                    // TODO: Remove FL_FILE information from exceptions?
                    CHECK_THROWS_AS(term->configure(parameters), fl::Exception);
                    CHECK_THROWS_WITH(term->configure(parameters), Catch::Matchers::StartsWith(expectedException));
                }
            }

            SECTION("parameterized") {
                // Can be constructed with requiredParameters
                std::vector<std::string> list;
                for (int parameter = 0; parameter < requiredParameters; ++parameter)
                    list.push_back("nan");

                std::string parameters;
                CAPTURE(parameters = fl::Op::join(list, " "));
                CHECK_NOTHROW(term->configure(parameters));

                SECTION("height") {
                    // Can be constructed with additional height parameter
                    if (height) {
                        list.push_back("nan");
                        CAPTURE(parameters = fl::Op::join(list, " "));
                        CHECK_NOTHROW(term->configure(parameters));
                    }
                }
            }

            // SECTION("overparameterized") {
            //     // Can't be constructed with more parameters
            //     std::vector<std::string> list;
            //     std::string parameters;
            //     for (int parameter = 0; parameter < requiredParameters + 1; ++parameter)
            //         list.push_back("nan");
            //     CAPTURE(parameters = fl::Op::join(list, " "));
            //     CHECK_THROWS_AS(term->configure(parameters), fl::Exception);
            //     CHECK_THROWS_WITH(term->configure(parameters), Catch::Matchers::StartsWith(expectedException));
            // }

            return *this;
        }

        TermAssert& configured_as(const std::string& parameters) {
            this->actual->configure(parameters);
            return *this;
        }

        TermAssert& has_memberships(
            const std::map<fl::scalar, fl::scalar>& values,
            const std::vector<scalar>& heights = {0.0, 0.25, .5, .75, 1.0}
        ) {
            for (scalar height : heights) {
                this->actual->setHeight(height);
                if (dynamic_cast<Constant*>(this->actual.get()))
                    height = 1.0;
                for (const auto& value : values) {
                    const auto& x = value.first;
                    const auto& expected_y = height * value.second;

                    const auto& obtained_y = this->actual->membership(x);
                    CAPTURE(x);
                    CAPTURE(height);
                    CHECK_THAT(obtained_y, Catch::Matchers::WithinAbs(expected_y, fuzzylite::macheps()));
                }
            }
            return *this;
        }

        TermAssert& has_tsukamotos(const std::map<fl::scalar, fl::scalar>& values) {
            CHECK(this->actual->isMonotonic());
            for (const auto& value : values) {
                const auto& x = value.first;
                const auto& expected_y = value.second;

                const auto& obtained_y = this->actual->tsukamoto(x, fl::nan, fl::nan);
                CAPTURE(x);
                CAPTURE(this->actual->getHeight());
                CHECK_THAT(obtained_y, Catch::Matchers::WithinAbs(expected_y, fuzzylite::macheps()));
            }
            return *this;
        }
    };

    TEST_CASE("All terms fail to raise exceptions with over-parameterization", "[term][!shouldfail]") {
        // TODO: Make sure terms raise exception with over-parameterization
        TermFactory* termFactory = fl::FactoryManager::instance()->term();
        const std::string& parameters = fl::Op::join(std::vector<std::string>(20, "nan"), " ");

        for (const std::string& termClass : termFactory->available()) {
            if (not(termClass == "" or termClass == "Discrete")) {
                const std::string& expectedException = "[configuration error] term <" + termClass + ">" + " requires <";
                Term* term = termFactory->constructObject(termClass);
                CAPTURE(parameters);
                CHECK_THROWS_AS(term->configure(parameters), fl::Exception);
                CHECK_THROWS_WITH(term->configure(parameters), Catch::Matchers::StartsWith(expectedException));
            }
        }
    }

    TEST_CASE("Bell", "[term][bell]") {
        TermAssert(new Bell("bell"))
            .takes_parameters(3)
            .exports_fll("term: bell Bell nan nan nan")
            .repr_is("fl.Bell('bell', fl.nan, fl.nan, fl.nan)")
            .is_not_monotonic()
            .configured_as("0 0.25 3.0")
            .exports_fll("term: bell Bell 0.000 0.250 3.000")
            .repr_is("fl.Bell('bell', 0.0, 0.25, 3.0)")
            .configured_as("0 0.25 3.0 0.5")
            .exports_fll("term: bell Bell 0.000 0.250 3.000 0.500")
            .repr_is("fl.Bell('bell', 0.0, 0.25, 3.0, 0.5)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.015384615384615385},
                {-0.4, 0.05625177755617076},
                {-0.25, 0.5},
                {-0.1, 0.9959207087768499},
                {0.0, 1.0},
                {0.1, 0.9959207087768499},
                {0.25, 0.5},
                {0.4, 0.05625177755617076},
                {0.5, 0.015384615384615385},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("Binary", "[term][binary]") {
        TermAssert(new Binary("binary"))
            .exports_fll("term: binary Binary nan nan")
            .repr_is("fl.Binary('binary', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic()
            .configured_as("0 inf")
            .exports_fll("term: binary Binary 0.000 inf")
            .repr_is("fl.Binary('binary', 0.0, fl.inf)")
            .configured_as("0 inf 0.5")
            .exports_fll("term: binary Binary 0.000 inf 0.500")
            .repr_is("fl.Binary('binary', 0.0, fl.inf, 0.5)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.0},
                {-0.4, 0.0},
                {-0.25, 0.0},
                {-0.1, 0.0},
                {0.0, 1.0},
                {0.1, 1.0},
                {0.25, 1.0},
                {0.4, 1.0},
                {0.5, 1.0},
                {1.0, 1.0},
                {fl::nan, fl::nan},
                {fl::inf, 1.0},
                {-fl::inf, 0.0},
            });
    }

    TEST_CASE("Concave Tsukamoto Bug", "[term][concave][!shouldfail]") {
        WARN("TODO: Fix Tsukamoto bugs");
        TermAssert(new Concave("concave"))
            .configured_as("0.00 -0.50")
            .exports_fll("term: concave Concave 0.000 -0.500")
            .has_tsukamotos({
                {0.0, inf},
                {0.25, 1.0},
                {0.5, 0.0},
                {0.75, -0.333},
                {1.0, -0.5},
                // #invalid value
                {-1.0, -1.5},
                {-0.5, -2.0},
                {nan, nan},
                {inf, -1},
                {-inf, -1},
            });

        TermAssert(new Concave("concave"))
            .configured_as("0.00 -0.500 0.5")
            .repr_is("fl.Concave('concave', 0.0, -0.5, 0.5)")
            .exports_fll("term: concave Concave 0.000 -0.500 0.500")
            .has_tsukamotos({
                {0.0, inf},
                {0.125, 1.0},
                {0.25, 0.0},
                {0.375, -0.333},
                {0.5, -0.5},
                // # invalid values
                {0.75, -0.666},
                {1.0, -0.75},
                {-1.0, -1.25},
                {-0.5, -1.5},
                {nan, nan},
                {inf, -1},
                {-inf, -1},
            });
    }

    TEST_CASE("Concave", "[term][concave]") {
        TermAssert(new Concave("concave"))
            .configured_as("0.00 -0.50")
            .exports_fll("term: concave Concave 0.000 -0.500")
            .has_memberships({
                {-1.0, 1.0},
                {-0.5, 1.0},
                {-0.4, 0.833},
                {-0.25, 0.666},
                {-0.1, 0.555},
                {0.0, 0.5},
                {0.1, 0.455},
                {0.25, 0.4},
                {0.4, 0.357},
                {0.5, 0.333},
                {1.0, 0.25},
                {nan, nan},
                {inf, 0.0},
                {-inf, 1.0},
            })
            // .has_tsukamotos({
            //     {0.0, inf},
            //     {0.25, 1.0},
            //     {0.5, 0.0},
            //     {0.75, -0.333},
            //     {1.0, -0.5},
            //     // #invalid value
            //     {-1.0, -1.5},
            //     {-0.5, -2.0},
            //     {nan, nan},
            //     {inf, -1},
            //     {-inf, -1},
            // });
            ;

        TermAssert(new Concave("concave"))
            .configured_as("0.00 -0.500 0.5")
            .repr_is("fl.Concave('concave', 0.0, -0.5, 0.5)")
            .exports_fll("term: concave Concave 0.000 -0.500 0.500")

            // .has_tsukamotos({
            //     {0.0, inf},
            //     {0.125, 1.0},
            //     {0.25, 0.0},
            //     {0.375, -0.333},
            //     {0.5, -0.5},
            //     // # invalid values
            //     {0.75, -0.666},
            //     {1.0, -0.75},
            //     {-1.0, -1.25},
            //     {-0.5, -1.5},
            //     {nan, nan},
            //     {inf, -1},
            //     {-inf, -1},
            // })
            ;
    }

    TEST_CASE("Constant", "[term][constant]") {
        TermAssert(new Constant("constant"))
            .exports_fll("term: constant Constant nan", false)
            .repr_is("fl.Constant('constant', fl.nan)")
            .takes_parameters(1, false)
            .is_not_monotonic()
            .configured_as("0.5")
            .exports_fll("term: constant Constant 0.500", false)
            .repr_is("fl.Constant('constant', 0.5)")
            .has_memberships({
                {-1.0, 0.5},
                {-0.5, 0.5},
                {-0.4, 0.5},
                {-0.25, 0.5},
                {-0.1, 0.5},
                {0.0, 0.5},
                {0.1, 0.5},
                {0.25, 0.5},
                {0.4, 0.5},
                {0.5, 0.5},
                {1.0, 0.5},
                {nan, 0.5},
                {inf, 0.5},
                {-inf, 0.5},
            })
            .configured_as("-0.500")
            .repr_is("fl.Constant('constant', -0.5)")
            .exports_fll("term: constant Constant -0.500", false)
            .has_memberships({
                {-1.0, -0.5},
                {-0.5, -0.5},
                {-0.4, -0.5},
                {-0.25, -0.5},
                {-0.1, -0.5},
                {0.0, -0.5},
                {0.1, -0.5},
                {0.25, -0.5},
                {0.4, -0.5},
                {0.5, -0.5},
                {1.0, -0.5},
                {nan, -0.5},
                {inf, -0.5},
                {-inf, -0.5},
            });
    }

    TEST_CASE("Cosine", "[term][cosine]") {
        TermAssert(new Cosine("cosine"))
            .exports_fll("term: cosine Cosine nan nan")
            .repr_is("fl.Cosine('cosine', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic()
            .configured_as("0.0 1")
            .exports_fll("term: cosine Cosine 0.000 1.000")
            .repr_is("fl.Cosine('cosine', 0.0, 1.0)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.0},
                {-0.4, 0.09549150281252633},
                {-0.25, 0.5},
                {-0.1, 0.9045084971874737},
                {0.0, 1.0},
                {0.1, 0.9045084971874737},
                {0.25, 0.5},
                {0.4, 0.09549150281252633},
                {0.5, 0.0},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("Discrete", "[term][discrete]") {
        // TODO
    }

    TEST_CASE("Gaussian", "[term][gaussian]") {
        TermAssert(new Gaussian("gaussian"))
            .exports_fll("term: gaussian Gaussian nan nan")
            .repr_is("fl.Gaussian('gaussian', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic()
            .configured_as("0.0 0.25")
            .exports_fll("term: gaussian Gaussian 0.000 0.250")
            .repr_is("fl.Gaussian('gaussian', 0.0, 0.25)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.1353352832366127},
                {-0.4, 0.2780373004531941},
                {-0.25, 0.6065306597126334},
                {-0.1, 0.9231163463866358},
                {0.0, 1.0},
                {0.1, 0.9231163463866358},
                {0.25, 0.6065306597126334},
                {0.4, 0.2780373004531941},
                {0.5, 0.1353352832366127},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("Gaussian Product", "[term][gaussian-product]") {
        TermAssert(new GaussianProduct("gaussian_product"))
            .exports_fll("term: gaussian_product GaussianProduct nan nan nan nan")
            .repr_is("fl.GaussianProduct('gaussian_product', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic()
            .configured_as("0.0 0.25 0.1 0.5")
            .repr_is("fl.GaussianProduct('gaussian_product', 0.0, 0.25, 0.1, 0.5)")
            .exports_fll("term: gaussian_product GaussianProduct 0.000 0.250 0.100 0.500")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.1353352832366127},
                {-0.4, 0.2780373004531941},
                {-0.25, 0.6065306597126334},
                {-0.1, 0.9231163463866358},
                {0.0, 1.0},
                {0.1, 1.0},
                {0.25, 0.9559974818331},
                {0.4, 0.835270211411272},
                {0.5, 0.7261490370736908},
                {1.0, 0.198},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("Linear", "[term][linear]") {
        // TODO
    }

    TEST_CASE("PiShape", "[term][pishape]") {
        TermAssert(new PiShape("pi_shape"))
            .exports_fll("term: pi_shape PiShape nan nan nan nan")
            .repr_is("fl.PiShape('pi_shape', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic()
            .configured_as("-.9 -.1 .1 1")
            .exports_fll("term: pi_shape PiShape -0.900 -0.100 0.100 1.000")
            .repr_is("fl.PiShape('pi_shape', -0.9, -0.1, 0.1, 1.0)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.5},
                {-0.4, 0.71875},
                {-0.25, 0.9296875},
                {-0.1, 1.0},
                {0.0, 1.0},
                {0.1, 1.0},
                {0.25, 0.9444444444444444},
                {0.4, 0.7777777777777777},
                {0.5, 0.6049382716049383},
                {0.95, 0.00617283950617285},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("Ramp", "[term][ramp]") {
        // TODO
    }

    TEST_CASE("Rectangle", "[term][rectangle]") {
        TermAssert(new Rectangle("rectangle"))
            .exports_fll("term: rectangle Rectangle nan nan")
            .repr_is("fl.Rectangle('rectangle', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic()
            .configured_as("-0.4 0.4")
            .exports_fll("term: rectangle Rectangle -0.400 0.400")
            .repr_is("fl.Rectangle('rectangle', -0.4, 0.4)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.0},
                {-0.4, 1.0},
                {-0.25, 1.0},
                {-0.1, 1.0},
                {0.0, 1.0},
                {0.1, 1.0},
                {0.25, 1.0},
                {0.4, 1.0},
                {0.5, 0.0},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

}  // namespace fl
