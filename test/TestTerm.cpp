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

/**
 * TODO
 * - Fix Tsukamoto tests
 * - Break builder tests
 * - Remove explicit height tests
 *
 */
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
            can_clone();

            // assert that configure with empty parameters does nothing
            const std::string& expected = this->actual->toString();
            this->actual->configure("");
            const std::string& obtained = this->actual->toString();
            CHECK_THAT(expected, Catch::Matchers::Equals(obtained));
            return *this;
        }

        TermAssert& can_clone() {
            const std::string& expected = this->actual->toString();
            TermAssert(this->actual->clone()).exports_fll(expected, false);
            return *this;
        }

        TermAssert& has_memberships(
            const std::map<fl::scalar, fl::scalar>& values,
            const scalar y_nan = fl::nan,
            const std::vector<scalar>& heights = {0.0, 0.25, .5, .75, 1.0}
        ) {
            for (scalar height : heights) {
                if (dynamic_cast<Constant*>(this->actual.get()) or dynamic_cast<Linear*>(this->actual.get()))
                    height = 1.0;
                this->actual->setHeight(height);
                for (const auto& value : values) {
                    const auto& x = value.first;
                    const auto& expected_y = height * value.second;

                    const auto& obtained_y = this->actual->membership(x);
                    CAPTURE(x);
                    CAPTURE(height);
                    if (fl::Op::isNaN(expected_y))
                        CHECK_THAT(fl::Op::str(obtained_y), Catch::Matchers::Equals(fl::Op::str(expected_y)));
                    else
                        CHECK_THAT(obtained_y, Catch::Matchers::WithinAbs(expected_y, fuzzylite::macheps()));
                }

                if (fl::Op::isNaN(y_nan))
                    CHECK_THAT(fl::Op::str(this->actual->membership(nan)), Catch::Matchers::Equals(fl::Op::str(y_nan)));
                else
                    CHECK_THAT(this->actual->membership(nan), Catch::Matchers::WithinAbs(y_nan, fuzzylite::macheps()));
            }
            return *this;
        }

        TermAssert& has_tsukamotos(const std::map<scalar, scalar>& values) {
            CAPTURE("Tsukamoto");
            CHECK(this->actual->isMonotonic());
            const scalar height = this->actual->getHeight();
            for (const auto& value : values) {
                const auto& x = value.first;
                const auto& expected_y = value.second;

                const auto& obtained_y = this->actual->tsukamoto(x, fl::nan, fl::nan);
                CAPTURE(x);
                CAPTURE(height);
                if (fl::Op::isNaN(expected_y))
                    CHECK_THAT(fl::Op::str(obtained_y), Catch::Matchers::Equals(fl::Op::str(expected_y)));
                else
                    CHECK_THAT(obtained_y, Catch::Matchers::WithinAbs(expected_y, fuzzylite::macheps()));
            }
            return *this;
        }

        TermAssert& apply(const std::function<void(Term*)>& f) {
            f(this->actual.get());
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
            .is_not_monotonic();

        TermAssert(new Bell("bell"))
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
            .is_not_monotonic();

        TermAssert(new Binary("binary"))
            .configured_as("0 inf")
            .exports_fll("term: binary Binary 0.000 inf")
            .repr_is("fl.Binary('binary', 0.0, fl.inf)")
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

    TEST_CASE("Concave", "[term][concave]") {
        TermAssert(new Concave("concave"))
            .exports_fll("term: concave Concave nan nan")
            .repr_is("fl.Concave('concave', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();

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
            });

        TermAssert(new Concave("concave"))
            .configured_as("0.00 -0.500 0.5")
            .repr_is("fl.Concave('concave', 0.0, -0.5, 0.5)")
            .exports_fll("term: concave Concave 0.000 -0.500 0.500");
    }

    TEST_CASE("Concave Tsukamoto Bug", "[term][concave][tsukamoto][!shouldfail]") {
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

    TEST_CASE("Constant", "[term][constant]") {
        TermAssert(new Constant("constant"))
            .exports_fll("term: constant Constant nan", false)
            .repr_is("fl.Constant('constant', fl.nan)")
            .takes_parameters(1, false)
            .is_not_monotonic();

        TermAssert(new Constant("constant"))
            .configured_as("0.5")
            .exports_fll("term: constant Constant 0.500", false)
            .repr_is("fl.Constant('constant', 0.5)")
            .has_memberships(
                {
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
                },
                0.5
            );
        TermAssert(new Constant("constant"))
            .configured_as("-0.500")
            .repr_is("fl.Constant('constant', -0.5)")
            .exports_fll("term: constant Constant -0.500", false)
            .has_memberships(
                {
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
                },
                -0.5
            );
    }

    TEST_CASE("Cosine", "[term][cosine]") {
        TermAssert(new Cosine("cosine"))
            .exports_fll("term: cosine Cosine nan nan")
            .repr_is("fl.Cosine('cosine', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic();

        TermAssert(new Cosine("cosine"))
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
        TermAssert(new Discrete("discrete"))
            .exports_fll("term: discrete Discrete")
            .repr_is("fl.Discrete('discrete')")
            .is_not_monotonic();

        TermAssert(new Discrete("discrete"))
            .configured_as("0 1 8 9 4 5 2 3 6 7")
            .exports_fll("term: discrete Discrete 0.000 1.000 8.000 9.000 4.000 5.000 2.000 3.000 6.000 7.000")
            .apply([](Term* term) -> void { dynamic_cast<Discrete*>(term)->sort(); })
            .exports_fll("term: discrete Discrete 0.000 1.000 2.000 3.000 4.000 5.000 6.000 7.000 8.000 9.000")
            .repr_is("fl.Discrete('discrete', fl.array([fl.array([0.0, 1.0]), fl.array([2.0, "
                     "3.0]), fl.array([4.0, 5.0]), fl.array([6.0, 7.0]), fl.array([8.0, 9.0])]))");

        CHECK_THROWS_WITH(
            Discrete("discrete").membership(0), Catch::Matchers::StartsWith("[discrete error] term is empty")
        );

        TermAssert(new Discrete("discrete"))
            .configured_as("0 1 8 9 4 5 2 3 6 7 0.5")
            .apply([](Term* term) -> void { dynamic_cast<Discrete*>(term)->sort(); })
            .exports_fll("term: discrete Discrete "
                         "0.000 1.000 2.000 3.000 4.000 5.000 6.000 7.000 8.000 9.000 0.500")
            .repr_is("fl.Discrete('discrete', fl.array([fl.array([0.0, 1.0]), fl.array([2.0, "
                     "3.0]), fl.array([4.0, 5.0]), fl.array([6.0, 7.0]), fl.array([8.0, 9.0])]), "
                     "0.5)")
            .configured_as(" -0.500 0.000 -0.250 1.000 0.000 0.500 0.250 1.000 0.500 0.000")
            .exports_fll("term: discrete Discrete "
                         "-0.500 0.000 -0.250 1.000 0.000 0.500 0.250 1.000 0.500 0.000")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.0},
                {-0.4, 0.3999999999999999},
                {-0.25, 1.0},
                {-0.1, 0.7},
                {0.0, 0.5},
                {0.1, 0.7},
                {0.25, 1.0},
                {0.4, 0.3999999999999999},
                {0.5, 0.0},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("Gaussian", "[term][gaussian]") {
        TermAssert(new Gaussian("gaussian"))
            .exports_fll("term: gaussian Gaussian nan nan")
            .repr_is("fl.Gaussian('gaussian', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic();
        TermAssert(new Gaussian("gaussian"))
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
            .is_not_monotonic();
        TermAssert(new GaussianProduct("gaussian_product"))
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
        Engine engine;
        engine.setInputVariables({new InputVariable("A"), new InputVariable("B"), new InputVariable("C")});
        engine.getInputVariable(0)->setValue(0);
        engine.getInputVariable(1)->setValue(1);
        engine.getInputVariable(2)->setValue(2);

        CHECK_THROWS_AS(Linear("linear").membership(nan), fl::Exception);
        CHECK_THROWS_WITH(
            Linear("linear").membership(nan),
            Catch::Matchers::StartsWith("[linear error] term <linear> is missing a reference to the engine")
        );

        Linear linear("linear", {1.0, 2.0, 3.0, 5.0, 8.0});
        CHECK(linear.getEngine() == fl::null);
        linear.updateReference(&engine);
        CHECK(linear.getEngine() == &engine);

        // TODO: Raise exception when mistmatch between coefficients and number of input variables
        // CHECK_THROWS_AS(linear.membership(0.0), fl::Exception);
        // CHECK_THROWS_WITH(
        //     linear.membership(0.0),
        //     Catch::Matchers::StartsWith(
        //         "expected 3 (+1) coefficients (one for each input variable plus an optional constant), "
        //         "but found 5 coefficients: [1.0, 2.0, 3.0, 5.0, 8.0]"
        //     )
        // );

        TermAssert(new Linear("linear", {1.0, 2.0}, &engine))
            .exports_fll("term: linear Linear 1.000 2.000", false)
            .repr_is("fl.Linear('linear', [1.0, 2.0])")
            .is_not_monotonic()
            .configured_as("1.0 2.0 3")
            .exports_fll("term: linear Linear 1.000 2.000 3.000", false)
            .repr_is("fl.Linear('linear', [1.0, 2.0, 3.0])")
            .has_memberships(
                {
                    {-1.0, 1 * 0 + 2 * 1 + 3 * 2},  // = 8
                    {-0.5, 8},
                    {-0.4, 8},
                    {-0.25, 8},
                    {-0.1, 8},
                    {0.0, 8},
                    {0.1, 8},
                    {0.25, 8},
                    {0.4, 8},
                    {0.5, 8},
                    {1.0, 8},
                    {nan, 8},
                    {inf, 8},
                    {-inf, 8},
                },
                8
            );
        TermAssert(new Linear("linear", {1.0, 2.0}, &engine))
            .configured_as("1 2 3 5")
            .exports_fll("term: linear Linear 1.000 2.000 3.000 5.000", false)
            .has_memberships(
                {
                    {-1.0, 1 * 0 + 2 * 1 + 3 * 2 + 5},  // = 1
                    {-0.5, 13},
                    {-0.4, 13},
                    {-0.25, 13},
                    {-0.1, 13},
                    {0.0, 13},
                    {0.1, 13},
                    {0.25, 13},
                    {0.4, 13},
                    {0.5, 13},
                    {1.0, 13},
                    {nan, 13},
                    {inf, 13},
                    {-inf, 13},
                },
                13
            );
    }

    TEST_CASE("PiShape", "[term][pishape]") {
        TermAssert(new PiShape("pi_shape"))
            .exports_fll("term: pi_shape PiShape nan nan nan nan")
            .repr_is("fl.PiShape('pi_shape', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();

        TermAssert(new PiShape("pi_shape"))
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
        TermAssert(new Ramp("ramp"))
            .exports_fll("term: ramp Ramp nan nan")
            .repr_is("fl.Ramp('ramp', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();

        TermAssert(new Ramp("ramp"))
            .configured_as("-0.250 0.750")
            .exports_fll("term: ramp Ramp -0.250 0.750")
            .repr_is("fl.Ramp('ramp', -0.25, 0.75)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.0},
                {-0.4, 0.0},
                {-0.25, 0.0},
                {-0.1, 0.15},
                {0.0, 0.25},
                {0.1, 0.35},
                {0.25, 0.50},
                {0.4, 0.65},
                {0.5, 0.75},
                {1.0, 1.0},
                {nan, nan},
                {inf, 1.0},
                {-inf, 0.0},
            });

        TermAssert(new Ramp("ramp"))
            .configured_as("0.250 -0.750")
            .exports_fll("term: ramp Ramp 0.250 -0.750")
            .has_memberships({
                {-1.0, 1.0},
                {-0.5, 0.750},
                {-0.4, 0.650},
                {-0.25, 0.500},
                {-0.1, 0.350},
                {0.0, 0.250},
                {0.1, 0.150},
                {0.25, 0.0},
                {0.4, 0.0},
                {0.5, 0.0},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 1.0},
            });

        TermAssert(new Ramp("ramp"))
            .configured_as("0.250 -0.750 0.5")
            .exports_fll("term: ramp Ramp 0.250 -0.750 0.500")
            .repr_is("fl.Ramp('ramp', 0.25, -0.75, 0.5)")
            .has_memberships({
                {-1.0, 1.0},
                {-0.5, 0.750},
                {-0.4, 0.650},
                {-0.25, 0.500},
                {-0.1, 0.350},
                {0.0, 0.250},
                {0.1, 0.150},
                {0.25, 0.0},
                {0.4, 0.0},
                {0.5, 0.0},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 1.0},
            });
    }

    TEST_CASE("Ramp Fix", "[term][ramp][!shouldfail]") {
        // TODO: fix ramp membership function when a=b
        TermAssert(new Ramp("ramp"))
            .configured_as("0 0")
            .exports_fll("term: ramp Ramp 0.000 0.000")
            .has_memberships({
                {-1.0, nan},
                {-0.5, nan},
                {-0.4, nan},
                {-0.25, nan},
                {-0.1, nan},
                {0.0, nan},
                {0.1, nan},
                {0.25, nan},
                {0.4, nan},
                {0.5, nan},
                {1.0, nan},
                {nan, nan},
                {inf, nan},
                {-inf, nan},
            });
    }

    TEST_CASE("Ramp Tsukamoto", "[term][ramp][tsukamoto]") {
        TermAssert(new Ramp("ramp"))
            .configured_as("0 0")
            .exports_fll("term: ramp Ramp 0.000 0.000")
            .has_tsukamotos({
                {0.0, 0.0},
                {0.5, 0.0},
                {1.0, 0.0},
                {nan, nan},
                {inf, nan},
                {-inf, nan},
            });

        TermAssert(new Ramp("ramp"))
            .configured_as("-0.250 0.750")
            .exports_fll("term: ramp Ramp -0.250 0.750")
            .repr_is("fl.Ramp('ramp', -0.25, 0.75)")
            .has_tsukamotos({
                {0.0, -0.25},
                {0.25, 0.0},
                {0.5, 0.25},
                {0.75, 0.5},
                {1.0, 0.75},
                // {# invalid value},
                {-1.0, -1.25},
                {-0.5, -0.75},
                {nan, nan},
                {inf, inf},
                {-inf, -inf},
            });

        TermAssert(new Ramp("ramp"))
            .configured_as("0.250 -0.750")
            .exports_fll("term: ramp Ramp 0.250 -0.750")
            .has_tsukamotos({
                {0.0, 0.25},
                {0.25, 0.0},
                {0.5, -0.25},
                {0.75, -0.5},
                {1.0, -0.75},
                // {# invalid value},
                {-1.0, 1.25},
                {-0.5, 0.75},
                {nan, nan},
                {inf, -inf},
                {-inf, inf},
            });
    }

    TEST_CASE("Ramp Tsukamoto Height", "[term][ramp][tsukamoto][!shouldfail]") {
        // TODO: Fix height in tsukamoto
        TermAssert(new Ramp("ramp"))
            .configured_as("0.250 -0.750 0.5")
            .exports_fll("term: ramp Ramp 0.250 -0.750 0.500")
            .repr_is("fl.Ramp('ramp', 0.25, -0.75, 0.5)")
            .has_tsukamotos({
                {0.0, 0.25},
                {0.125, 0},
                {0.25, -0.25},
                {0.375, -0.5},
                {0.5, -0.75},
                // {# invalid value},
                {0.75, -1.25},
                {1.0, -1.75},
                {-1.0, 2.25},
                {-0.5, 1.25},
                {nan, nan},
                {inf, -inf},
                {-inf, inf},
            });
    }

    TEST_CASE("Rectangle", "[term][rectangle]") {
        TermAssert(new Rectangle("rectangle"))
            .exports_fll("term: rectangle Rectangle nan nan")
            .repr_is("fl.Rectangle('rectangle', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic();
        TermAssert(new Rectangle("rectangle"))
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

    TEST_CASE("Sigmoid", "[term][sigmoid]") {
        TermAssert(new Sigmoid("sigmoid"))
            .exports_fll("term: sigmoid Sigmoid nan nan")
            .repr_is("fl.Sigmoid('sigmoid', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();

        TermAssert(new Sigmoid("sigmoid"))
            .configured_as("0 10")
            .exports_fll("term: sigmoid Sigmoid 0.000 10.000")
            .repr_is("fl.Sigmoid('sigmoid', 0.0, 10.0)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.007},
                {-0.4, 0.018},
                {-0.25, 0.076},
                {-0.1, 0.269},
                {0.0, 0.5},
                {0.1, 0.731},
                {0.25, 0.924},
                {0.4, 0.982},
                {0.5, 0.993},
                {1.0, 0.999},
                {nan, nan},
                {inf, 1.0},
                {-inf, 0.0},
            });

        TermAssert(new Sigmoid("sigmoid"))
            .configured_as("0 -10")
            .exports_fll("term: sigmoid Sigmoid 0.000 -10.000")
            .has_memberships({
                {-1.0, 0.999},
                {-0.5, 0.994},
                {-0.4, 0.982},
                {-0.25, 0.924},
                {-0.1, 0.731},
                {0.0, 0.5},
                {0.1, 0.269},
                {0.25, 0.076},
                {0.4, 0.018},
                {0.5, 0.007},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 1.0},
            });
    }

    TEST_CASE("Sigmoid Tsukamoto", "[term][sigmoid][tsukamoto][!shouldfail]") {
        // TODO: Fix min-max in tsukamoto signature
        TermAssert(new Sigmoid("sigmoid"))
            .configured_as("0 -10")
            .exports_fll("term: sigmoid Sigmoid 0.000 -10.000")
            .has_tsukamotos({
                {0.0, inf},
                {0.25, 0.109},
                {0.5, 0.0},
                {0.75, -0.109},
                {1.0, -inf},
                // # invalid values,
                {-1.0, nan},
                {-0.5, nan},
                {nan, nan},
                {inf, nan},
                {-inf, nan},
            });

        TermAssert(new Sigmoid("sigmoid"))
            .configured_as("0 10")
            .exports_fll("term: sigmoid Sigmoid 0.000 10.000")
            .repr_is("fl.Sigmoid('sigmoid', 0.0, 10.0)")
            .has_tsukamotos({
                {0.0, -inf},
                {0.25, -0.109},
                {0.5, 0.0},
                {0.75, 0.109},
                {1.0, inf},
                // {# invalid value},
                {-1.0, nan},
                {-0.5, nan},
                {nan, nan},
                {inf, nan},
                {-inf, nan},
            });
    }

    TEST_CASE("SigmoidDifference", "[term][sigmoid-difference]") {
        TermAssert(new SigmoidDifference("sigmoid_difference"))
            .exports_fll("term: sigmoid_difference SigmoidDifference nan nan nan nan")
            .repr_is("fl.SigmoidDifference('sigmoid_difference', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();

        TermAssert(new SigmoidDifference("sigmoid_difference"))
            .configured_as("-0.25 25.00 50.00 0.25")
            .exports_fll("term: sigmoid_difference SigmoidDifference -0.250 25.000 50.000 0.250")
            .repr_is("fl.SigmoidDifference('sigmoid_difference', -0.25, 25.0, 50.0, 0.25)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.0019267346633274238},
                {-0.4, 0.022977369910017923},
                {-0.25, 0.49999999998611205},
                {-0.1, 0.9770226049799834},
                {0.0, 0.9980695386973883},
                {0.1, 0.9992887851439739},
                {0.25, 0.49999627336071584},
                {0.4, 0.000552690994449101},
                {0.5, 3.7194451510957904e-06},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("SigmoidProduct", "[term][sigmoid-product]") {
        TermAssert(new SigmoidProduct("sigmoid_product"))
            .exports_fll("term: sigmoid_product SigmoidProduct nan nan nan nan")
            .repr_is("fl.SigmoidProduct('sigmoid_product', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();
        TermAssert(new SigmoidProduct("sigmoid_product"))
            .configured_as("-0.250 20.000 -20.000 0.250")
            .exports_fll("term: sigmoid_product SigmoidProduct -0.250 20.000 -20.000 0.250")
            .repr_is("fl.SigmoidProduct('sigmoid_product', -0.25, 20.0, -20.0, 0.25)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.006692848876926853},
                {-0.4, 0.04742576597971327},
                {-0.25, 0.4999773010656488},
                {-0.1, 0.9517062830264366},
                {0.0, 0.9866590924049252},
                {0.1, 0.9517062830264366},
                {0.25, 0.4999773010656488},
                {0.4, 0.04742576597971327},
                {0.5, 0.006692848876926853},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("Spike", "[term][spike]") {
        TermAssert(new Spike("spike"))
            .exports_fll("term: spike Spike nan nan")
            .repr_is("fl.Spike('spike', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic();
        TermAssert(new Spike("spike"))
            .configured_as("0 1.0")
            .exports_fll("term: spike Spike 0.000 1.000")
            .repr_is("fl.Spike('spike', 0.0, 1.0)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.006737946999085467},
                {-0.4, 0.01831563888873418},
                {-0.25, 0.0820849986238988},
                {-0.1, 0.36787944117144233},
                {0.0, 1.0},
                {0.1, 0.36787944117144233},
                {0.25, 0.0820849986238988},
                {0.4, 0.01831563888873418},
                {0.5, 0.006737946999085467},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("SShape", "[term][sshape]") {
        TermAssert(new SShape("s_shape"))
            .exports_fll("term: s_shape SShape nan nan")
            .repr_is("fl.SShape('s_shape', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();
        TermAssert(new SShape("s_shape"))
            .configured_as("-0.5 0.5")
            .exports_fll("term: s_shape SShape -0.500 0.500")
            .repr_is("fl.SShape('s_shape', -0.5, 0.5)")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.0},
                {-0.4, 0.02},
                {-0.25, 0.125},
                {-0.1, 0.32},
                {0.0, 0.5},
                {0.1, 0.68},
                {0.25, 0.875},
                {0.4, 0.98},
                {0.5, 1.0},
                {1.0, 1.0},
                {nan, nan},
                {inf, 1.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("SShape Tsukamoto", "[term][sshape][tsukamoto][!shouldfail]") {
        // TODO: Fix bug
        TermAssert(new SShape("s_shape"))
            .exports_fll("term: s_shape SShape nan nan")
            .repr_is("fl.SShape('s_shape', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();
        TermAssert(new SShape("s_shape"))
            .configured_as("-0.5 0.5")
            .exports_fll("term: s_shape SShape -0.500 0.500")
            .repr_is("fl.SShape('s_shape', -0.5, 0.5)")
            .has_tsukamotos({
                {0.0, -0.5},
                {0.25, -0.146},
                {0.5, 0.0},
                {0.75, 0.146},
                {1.0, 0.5},
                // invalid values
                {-1.0, nan},
                {-0.5, nan},
                {nan, nan},
                {inf, nan},
                {-inf, nan},
            });

        TermAssert(new SShape("s_shape"))
            .configured_as("-0.5 0.5 0.5")
            .repr_is("fl.SShape('s_shape', -0.5, 0.5, 0.5)")
            .exports_fll("term: s_shape SShape -0.500 0.500 0.500")
            .has_tsukamotos({
                {0.0, -0.5},
                {0.125, -0.146},
                {0.25, 0},
                {0.375, 0.146},
                {0.5, 0.5},
                // invalid values
                {0.75, nan},
                {1.0, nan},
                {-1.0, nan},
                {-0.5, nan},
                {nan, nan},
                {inf, nan},
                {-inf, nan},
            });
    }

    TEST_CASE("Trapezoid", "[term][trapezoid]") {
        TermAssert(new Trapezoid("trapezoid"))
            .exports_fll("term: trapezoid Trapezoid nan nan nan nan")
            .repr_is("Trapezoid('trapezoid', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();

        TermAssert(new Trapezoid("trapezoid", 0.0, 1.0))
            .exports_fll("term: trapezoid Trapezoid 0.000 0.200 0.800 1.000")
            .repr_is("Trapezoid('trapezoid', 0.0, 0.2, 0.8, 1.0)");

        TermAssert(new Trapezoid("trapezoid"))
            .configured_as("-0.400 -0.100 0.100 0.400")
            .exports_fll("term: trapezoid Trapezoid -0.400 -0.100 0.100 0.400")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.000},
                {-0.25, 0.500},
                {-0.1, 1.000},
                {0.0, 1.000},
                {0.1, 1.000},
                {0.25, 0.500},
                {0.4, 0.000},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
        TermAssert(new Trapezoid("trapezoid"))
            .configured_as("-0.400 -0.400 0.100 0.400")
            .exports_fll("term: trapezoid Trapezoid -0.400 -0.400 0.100 0.400")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 1.000},
                {-0.25, 1.000},
                {-0.1, 1.000},
                {0.0, 1.000},
                {0.1, 1.000},
                {0.25, 0.500},
                {0.4, 0.000},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
        TermAssert(new Trapezoid("trapezoid"))
            .configured_as("-0.400 -0.100 0.400 0.400")
            .exports_fll("term: trapezoid Trapezoid -0.400 -0.100 0.400 0.400")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.000},
                {-0.25, 0.5},
                {-0.1, 1.000},
                {0.0, 1.000},
                {0.1, 1.000},
                {0.25, 1.000},
                {0.4, 1.000},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
        TermAssert(new Trapezoid("trapezoid"))
            .configured_as("-inf -0.100 0.100 .4")
            .exports_fll("term: trapezoid Trapezoid -inf -0.100 0.100 0.400")
            .has_memberships({
                {-1.0, 1.0},
                {-0.5, 1.000},
                {-0.4, 1.000},
                {-0.25, 1.000},
                {-0.1, 1.000},
                {0.0, 1.000},
                {0.1, 1.000},
                {0.25, 0.500},
                {0.4, 0.000},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 1.0},
            });
        TermAssert(new Trapezoid("trapezoid"))
            .configured_as("-.4 -0.100 0.100 inf .5")
            .exports_fll("term: trapezoid Trapezoid -0.400 -0.100 0.100 inf 0.500")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.000},
                {-0.25, 0.500},
                {-0.1, 1.000},
                {0.0, 1.000},
                {0.1, 1.000},
                {0.25, 1.000},
                {0.4, 1.000},
                {0.5, 1.000},
                {1.0, 1.0},
                {nan, nan},
                {inf, 1.0},
                {-inf, 0.0},
            });
    }

    TEST_CASE("Triangle", "[term][triangle]") {
        TermAssert(new Triangle("triangle"))
            .exports_fll("term: triangle Triangle nan nan nan")
            .repr_is("fl.Triangle('triangle', fl.nan, fl.nan, fl.nan)")
            .takes_parameters(3)
            .is_not_monotonic();

        TermAssert(new Triangle("triangle", 0.0, 1.0))
            .exports_fll("term: triangle Triangle 0.000 0.500 1.000")
            .repr_is("fl.Triangle('triangle', 0.0, 0.5, 1.0)");

        TermAssert(new Triangle("triangle"))
            .configured_as("-0.400 0.000 0.400")
            .exports_fll("term: triangle Triangle -0.400 0.000 0.400")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.000},
                {-0.25, 0.37500000000000006},
                {-0.1, 0.7500000000000001},
                {0.0, 1.000},
                {0.1, 0.7500000000000001},
                {0.25, 0.37500000000000006},
                {0.4, 0.000},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
        TermAssert(new Triangle("triangle"))
            .configured_as("-0.400 0.000 0.400 .5")
            .repr_is("fl.Triangle('triangle', -0.4, 0.0, 0.4, 0.5)")
            .exports_fll("term: triangle Triangle -0.400 0.000 0.400 0.500")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.000},
                {-0.25, 0.37500000000000006},
                {-0.1, 0.7500000000000001},
                {0.0, 1.000},
                {0.1, 0.7500000000000001},
                {0.25, 0.37500000000000006},
                {0.4, 0.000},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
        TermAssert(new Triangle("triangle"))
            .configured_as("-0.500 0.000 0.500")
            .exports_fll("term: triangle Triangle -0.500 0.000 0.500")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.2},
                {-0.25, 0.5},
                {-0.1, 0.8},
                {0.0, 1.000},
                {0.1, 0.8},
                {0.25, 0.5},
                {0.4, 0.2},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
        TermAssert(new Triangle("triangle"))
            .configured_as("-0.500 -0.500 0.500")
            .exports_fll("term: triangle Triangle -0.500 -0.500 0.500")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 1.000},
                {-0.4, 0.900},
                {-0.25, 0.75},
                {-0.1, 0.6},
                {0.0, 0.5},
                {0.1, 0.4},
                {0.25, 0.25},
                {0.4, 0.1},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
        TermAssert(new Triangle("triangle"))
            .configured_as("-0.500 0.500 0.500")
            .exports_fll("term: triangle Triangle -0.500 0.500 0.500")
            .has_memberships({
                {1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.1},
                {-0.25, 0.25},
                {-0.1, 0.4},
                {0.0, 0.5},
                {0.1, 0.6},
                {0.25, 0.75},
                {0.4, 0.900},
                {0.5, 1.000},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });
        TermAssert(new Triangle("triangle"))
            .configured_as("-inf 0.000 0.400")
            .exports_fll("term: triangle Triangle -inf 0.000 0.400")
            .has_memberships({
                {-1.0, 1.0},
                {-0.5, 1.000},
                {-0.4, 1.000},
                {-0.25, 1.000},
                {-0.1, 1.000},
                {0.0, 1.000},
                {0.1, 0.75},
                {0.25, 0.375},
                {0.4, 0.000},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 1.000},
            });
        TermAssert(new Triangle("triangle"))
            .configured_as("-0.400 0.000 inf .5")
            .exports_fll("term: triangle Triangle -0.400 0.000 inf 0.500")
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.000},
                {-0.25, 0.375},
                {-0.1, 0.750},
                {0.0, 1.000},
                {0.1, 1.000},
                {0.25, 1.000},
                {0.4, 1.000},
                {0.5, 1.000},
                {1.0, 1.0},
                {nan, nan},
                {inf, 1.000},
                {-inf, 0.0},
            });
    }

    TEST_CASE("ZShape", "[term][zshape]") {
        TermAssert(new ZShape("z_shape"))
            .exports_fll("term: z_shape ZShape nan nan")
            .repr_is("fl.ZShape('z_shape', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();

        TermAssert(new ZShape("z_shape"))
            .configured_as("-0.5 0.5")
            .exports_fll("term: z_shape ZShape -0.500 0.500")
            .repr_is("fl.ZShape('z_shape', -0.5, 0.5)")
            .has_memberships({
                {-1.0, 1.0},
                {-0.5, 1.0},
                {-0.4, 0.98},
                {-0.25, 0.875},
                {-0.1, 0.68},
                {0.0, 0.5},
                {0.1, 0.32},
                {0.25, 0.125},
                {0.4, 0.02},
                {0.5, 0.0},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 1.0},
            });

        TermAssert(new ZShape("z_shape"))
            .configured_as("-0.5 0.5 0.5")
            .repr_is("fl.ZShape('z_shape', -0.5, 0.5, 0.5)")
            .exports_fll("term: z_shape ZShape -0.500 0.500 0.500")
            .has_memberships({
                {-1.0, 1.0},
                {-0.5, 1.0},
                {-0.4, 0.98},
                {-0.25, 0.875},
                {-0.1, 0.68},
                {0.0, 0.5},
                {0.1, 0.32},
                {0.25, 0.125},
                {0.4, 0.02},
                {0.5, 0.0},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 1.0},
            });
    }

    TEST_CASE("ZShape Tsukamoto", "[term][zshape][tsukamoto][!shouldfail]") {
        // TODO: fix bug
        TermAssert(new ZShape("z_shape"))
            .exports_fll("term: z_shape ZShape nan nan")
            .repr_is("fl.ZShape('z_shape', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();

        TermAssert(new ZShape("z_shape"))
            .configured_as("-0.5 0.5")
            .exports_fll("term: z_shape ZShape -0.500 0.500")
            .repr_is("fl.ZShape('z_shape', -0.5, 0.5)")
            .has_tsukamotos({
                {0.0, 0.5},
                {0.25, 0.146},
                {0.5, 0.0},
                {0.75, -0.146},
                {1.0, -0.5},
                // invalid values
                {-1.0, nan},
                {-0.5, nan},
                {nan, nan},
                {inf, nan},
                {-inf, nan},
            });

        TermAssert(new ZShape("z_shape"))
            .configured_as("-0.5 0.5 0.5")
            .repr_is("fl.ZShape('z_shape', -0.5, 0.5, 0.5)")
            .exports_fll("term: z_shape ZShape -0.500 0.500 0.500")
            .has_tsukamotos({
                {0.0, 0.5},
                {0.125, 0.146},
                {0.25, 0.0},
                {0.375, -0.146},
                {0.5, -0.5},
                // invalid values
                {0.75, nan},
                {1.0, nan},
                {-1.0, nan},
                {-0.5, nan},
                {nan, nan},
                {inf, nan},
                {-inf, nan},
            });
    }

}  // namespace fl
