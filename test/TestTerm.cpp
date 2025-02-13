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

    template <typename T>
    struct TermAssert {
        std::unique_ptr<T> actual;

        TermAssert() : actual(std::make_unique<T>()) {}

        TermAssert(T* pointer) : actual(pointer) {}

        template <typename... Args>
        explicit TermAssert(Args&&... args) : actual(std::make_unique<T>(std::forward<Args>(args)...)) {}

        /**
         *
         * @deprecated T::constructor will be removed in fuzzylite 8
         */
        TermAssert& can_construct() {
            std::unique_ptr<T> test(dynamic_cast<T*>(T::constructor()));
            test->setName(actual->getName());
            test->configure(actual->parameters());
            CHECK(FllExporter().toString(test.get()) == FllExporter().toString(actual.get()));
            return *this;
        }

        TermAssert& can_clone() {
            // throw  Exception("");
            std::unique_ptr<T> test(actual->clone());
            std::string expected = FllExporter().toString(actual.get());
            std::string obtained = FllExporter().toString(test.get());
            CAPTURE(expected, obtained);
            CHECK(expected == obtained);
            return *this;
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
            std::unique_ptr<Term> term(termFactory->constructObject(this->actual->className()));
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

            /**
             * @deprecated fuzzylite 8 will raise exception when overparameterised.
             * expected failure created later
             */
            SECTION("overparameterized") {
                // Can't be constructed with more parameters
                std::vector<std::string> list;
                std::string parameters;
                for (int parameter = 0; parameter < requiredParameters + 1; ++parameter)
                    list.push_back("nan");
                CAPTURE(parameters = fl::Op::join(list, " "));
                // CHECK_THROWS_AS(term->configure(parameters), fl::Exception);
                // CHECK_THROWS_WITH(term->configure(parameters), Catch::Matchers::StartsWith(expectedException));
            }

            return *this;
        }

        TermAssert& configured_as(const std::string& parameters, bool checkEmpty = true) {
            this->actual->configure(parameters);

            if (checkEmpty) {
                // assert that configure with empty parameters does nothing
                const std::string& expected = this->actual->toString();
                this->actual->configure("");
                const std::string& obtained = this->actual->toString();
                CHECK_THAT(expected, Catch::Matchers::Equals(obtained));
            }
            return *this;
        }

        TermAssert& has_memberships(
            const std::vector<std::vector<scalar>>& values,
            const std::vector<scalar>& heights = {0.0, 0.25, .5, .75, 1.0}
        ) {
            for (scalar height : heights) {
                if (dynamic_cast<Constant*>(this->actual.get()) or dynamic_cast<Linear*>(this->actual.get()))
                    height = 1.0;
                this->actual->setHeight(height);
                for (const auto& value : values) {
                    CHECK(value.size() == 2);
                    const auto& x = value.front();
                    const auto& expected_y = height * value.back();

                    const auto& obtained_y = this->actual->membership(x);
                    CAPTURE(x);
                    CAPTURE(height);
                    CHECK_THAT(obtained_y, Approximates(expected_y));
                }
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

                const auto& obtained_y = this->actual->tsukamoto(x);
                CAPTURE(x);
                CAPTURE(height);
                CHECK_THAT(obtained_y, Approximates(expected_y));
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
            if (not(termClass.empty() or termClass == "Discrete")) {
                const std::string& expectedException = "[configuration error] term <" + termClass + ">" + " requires <";
                Term* term = termFactory->constructObject(termClass);
                CAPTURE(parameters);
                CHECK_THROWS_AS(term->configure(parameters), fl::Exception);
                CHECK_THROWS_WITH(term->configure(parameters), Catch::Matchers::StartsWith(expectedException));
            }
        }
    }

    TEST_CASE("All terms can copy and move", "[term][constructor]") {
        // TermFactory* termFactory = fl::FactoryManager::instance()->term();
        // const std::string& parameters = fl::Op::join(std::vector<scalar>{1., 2., 3., 4., 5., 6., 7., 8.}, " ");
        // std::vector<std::string> o;
        // for (const std::string& termClass : termFactory->available()) {
        //     if (not(termClass.empty()) and termClass != "Function") {
        //         FL_LOG(termClass);
        //
        //         auto term = termFactory->constructObject(termClass);
        //         term->setName(termClass);
        //         term->configure(parameters);
        //         term->setHeight(.5);
        //         auto c = CppExporter().toString(term);
        //         c = Op::replaceAll(c, "new ", "");
        //         c = Op::replaceAll(c, "(", "{");
        //         c = Op::replaceAll(c, ")", "}");
        //         o.push_back("AssertConstructor().can_copy_and_move(" + c + ", compare);");
        //     }
        // }
        // o.push_back("AssertConstructor().can_copy_and_move(Function{\"Function\", \"1.0 + 2.0\"}, compare);");
        // FL_LOG(Op::join(o, "\n"));
        auto compare = &Term::toString;
        auto triangle = std::make_unique<Triangle>("triangle", 1.0, 2.0, 3.0);
        auto minimum = std::make_unique<Minimum>();
        AssertConstructor().can_copy_and_move(Activated{triangle.get(), 0.5, minimum.get()}, compare);
        AssertConstructor().can_copy_and_move(
            Aggregated{"Aggregated", -1., 1., new Maximum, {Activated{triangle.get(), 0.5, minimum.get()}}}, compare
        );
        AssertConstructor().can_copy_and_move(Bell{"Bell", 1.000, 2.000, 3.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(Binary{"Binary", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(Concave{"Concave", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(Constant{"Constant", 1.000}, compare);
        AssertConstructor().can_copy_and_move(Cosine{"Cosine", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(
            Discrete{"Discrete", {{1.000, 2.000}, {3.000, 4.000}, {5.000, 6.000}, {7.000, 8.000}}, .5}, compare
        );
        AssertConstructor().can_copy_and_move(Function{"Function", "1.0 + 2.0"}, compare);
        AssertConstructor().can_copy_and_move(Gaussian{"Gaussian", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(
            GaussianProduct{"GaussianProduct", 1.000, 2.000, 3.000, 4.000, 0.500}, compare
        );
        AssertConstructor().can_copy_and_move(
            Linear{"Linear", {1.000, 2.000, 3.000, 4.000, 5.000, 6.000, 7.000, 8.000}}, compare
        );
        AssertConstructor().can_copy_and_move(PiShape{"PiShape", 1.000, 2.000, 3.000, 4.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(Ramp{"Ramp", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(Rectangle{"Rectangle", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(SShape{"SShape", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(Sigmoid{"Sigmoid", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(
            SigmoidDifference{"SigmoidDifference", 1.000, 2.000, 3.000, 4.000, 0.500}, compare
        );
        AssertConstructor().can_copy_and_move(
            SigmoidProduct{"SigmoidProduct", 1.000, 2.000, 3.000, 4.000, 0.500}, compare
        );
        AssertConstructor().can_copy_and_move(Spike{"Spike", 1.000, 2.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(Trapezoid{"Trapezoid", 1.000, 2.000, 3.000, 4.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(Triangle{"Triangle", 1.000, 2.000, 3.000, 0.500}, compare);
        AssertConstructor().can_copy_and_move(ZShape{"ZShape", 1.000, 2.000, 0.500}, compare);
    }

    TEST_CASE("Activated", "[term][activated]") {
        SECTION("Can clone") {
            const Triangle triangle("A", 0, 1, 2);
            const Minimum minimum;
            TermAssert<Activated>(&triangle, 0.5, &minimum).can_clone();
        }
        const AlgebraicProduct algebraicProduct;
        Triangle term("triangle", -0.400, 0.000, 0.400);
        TermAssert<Activated>(&term, 1.0, &algebraicProduct)
            .repr_is("fl.Activated(term=fl.Triangle('triangle', -0.4, 0.0, 0.4), "
                     "degree=1.0, implication=fl.AlgebraicProduct())")
            .exports_fll("AlgebraicProduct(1.000,triangle)", false)
            .is_not_monotonic()
            .has_memberships({
                {-1.0, 0.0},
                {-0.5, 0.000},
                {-0.4, 0.000},
                {-0.25, 0.375},
                {-0.1, 0.750},
                {0.0, 1.000},
                {0.1, 0.750},
                {0.25, 0.375},
                {0.4, 0.000},
                {0.5, 0.000},
                {1.0, 0.0},
                {nan, nan},
                {inf, 0.0},
                {-inf, 0.0},
            });

        TermAssert<Activated>(&term, 0.5, &algebraicProduct)
            .repr_is("fl.Activated(term=fl.Triangle('triangle', -0.4, 0.0, 0.4), "
                     "degree=0.5, implication=fl.AlgebraicProduct())")
            .exports_fll("AlgebraicProduct(0.500,triangle)", false)
            .configured_as("###")
            .exports_fll("AlgebraicProduct(0.500,triangle)");

        CHECK(Activated(&term, 0.5).toString() == "(0.500*triangle)");
        CHECK(Activated(&term, 0.5).parameters() == "0.500 none term: triangle Triangle -0.400 0.000 0.400");

        CHECK(Activated(&term, 0.5).fuzzyValue() == "+0.500/triangle");
        CHECK(Activated(&term, 0).fuzzyValue() == "+0.000/triangle");
        CHECK(Activated(&term, -0.5).fuzzyValue() == "-0.500/triangle");
        CHECK(Activated(&term, inf).fuzzyValue() == "+inf/triangle");
        CHECK(Activated(&term, -inf).fuzzyValue() == "-inf/triangle");
        CHECK(Activated(&term, nan).fuzzyValue() == "+nan/triangle");

        Activated activated;
        CHECK(activated.className() == "Activated");
        CHECK(activated.getName() == "");
        CHECK(activated.getImplication() == fl::null);
        CHECK(activated.getTerm() == fl::null);

        activated.setImplication(&algebraicProduct);
        activated.setTerm(&term);
        CHECK(activated.getImplication() == &algebraicProduct);
        CHECK(activated.getTerm() == &term);
        CHECK(activated.getName() == "triangle");

        Ramp ramp("ramp", 0, 1);
        CHECK(Activated(&ramp).isMonotonic());

        CHECK_THROWS_AS(Activated().membership(0.0), fl::Exception);
        CHECK_THROWS_WITH(
            Activated().membership(0.0), Catch::Matchers::StartsWith("[activation error] no term available to activate")
        );

        CHECK_THROWS_AS(Activated(&term).membership(0.0), fl::Exception);
        CHECK_THROWS_WITH(
            Activated(&term).membership(0.0),
            Catch::Matchers::StartsWith("[implication error] implication operator needed to activate")
        );
    }

    TEST_CASE("Aggregated", "[term][aggregated]") {
        SECTION("Can clone") {
            const Triangle triangle("A", 0, 1, 2);
            Maximum maximum;
            TermAssert<Aggregated>(
                "agg",
                0.0,
                10.0,
                new Maximum,
                std::vector<Activated>{
                    Activated(&triangle, 0.5),
                    Activated(&triangle, 0.25),
                }
            )
                .can_clone();
        }
        Minimum minimum;
        Aggregated aggregated("fuzzy_output", -1., 1.0, new Maximum);
        Triangle low("LOW", -1.000, -0.500, 0.000);
        Triangle medium("MEDIUM", -0.500, 0.000, 0.500);
        aggregated.setTerms({
            Activated(&low, 0.6, &minimum),
            Activated(&medium, 0.4, &minimum),
        });

        TermAssert<Aggregated>(aggregated)
            .exports_fll("fuzzy_output: Aggregated Maximum[Minimum(0.600,LOW),Minimum(0.400,MEDIUM)]", false)
            .repr_is("fl.Aggregated(name='fuzzy_output', minimum=-1.0, maximum=1.0, "
                     "aggregation=fl.Maximum(), terms=[fl.Activated(term=fl.Triangle('LOW', -1.0, "
                     "-0.5, 0.0), degree=0.6, implication=fl.Minimum()), "
                     "fl.Activated(term=fl.Triangle('MEDIUM', -0.5, 0.0, 0.5), degree=0.4, "
                     "implication=fl.Minimum())])")
            .is_not_monotonic()
            .has_memberships(
                {
                    {-1.0, 0.0},
                    {-0.5, 0.6},
                    {-0.4, 0.6},
                    {-0.25, 0.5},
                    {-0.1, 0.4},
                    {0.0, 0.4},
                    {0.1, 0.4},
                    {0.25, 0.4},
                    {0.4, 0.2},
                    {0.5, 0.0},
                    {1.0, 0.0},
                    {nan, nan},
                    {inf, 0.0},
                    {-inf, 0.0},
                },
                {1.0}
            )
            .configured_as("###")
            .exports_fll("fuzzy_output: Aggregated Maximum[Minimum(0.600,LOW),Minimum(0.400,MEDIUM)]", false);

        CHECK_THAT(aggregated.activationDegree(&low), Catch::Matchers::WithinAbs(0.6, fuzzylite::macheps()));
        CHECK_THAT(aggregated.activationDegree(&medium), Catch::Matchers::WithinAbs(0.4, fuzzylite::macheps()));
        CHECK(aggregated.highestActivatedTerm().getTerm() == &low);

        aggregated.setRange(-2, 2);
        CHECK(aggregated.range() == 4);

        aggregated.addTerm(Activated(&low, 0.4, &minimum));
        CHECK(
            aggregated.parameters()
            == "Maximum -2.000 2.000 "
               "term: LOW Activated 0.600 Minimum "
               "term: LOW Triangle -1.000 -0.500 0.000 "
               "term: MEDIUM Activated 0.400 Minimum "
               "term: MEDIUM Triangle -0.500 0.000 0.500 "
               "term: LOW Activated 0.400 Minimum "
               "term: LOW Triangle -1.000 -0.500 0.000"
        );

        aggregated.setAggregation(fl::null);
        CHECK(aggregated.activationDegree(&low) == 1.0);

        Aggregated copy;
        copy = aggregated;
        CHECK(copy.toString() == aggregated.toString());
        copy.setName("X");
        CHECK(copy.getName() != aggregated.getName());

        CHECK(aggregated.terms().back().getName() == "LOW");
        aggregated.removeTerm(aggregated.terms().size() - 1);
        CHECK(aggregated.terms().back().getName() == "MEDIUM");

        CHECK_THROWS_AS(aggregated.membership(0.0), fl::Exception);
        CHECK_THROWS_WITH(
            aggregated.membership(0.0),
            Catch::Matchers::StartsWith("[aggregation error] aggregation operator needed to aggregate variable")
        );
    }

    TEST_CASE("Aggregated: highest term", "[term][aggregated]") {
        std::unique_ptr<Term> dark(new Triangle("DARK", 0.000, 0.250, 0.500));
        std::unique_ptr<Term> medium(new Triangle("MEDIUM", 0.250, 0.500, 0.750));
        std::unique_ptr<Term> bright(new Triangle("BRIGHT", 0.500, 0.750, 1.000));

        Aggregated aggregated;
        aggregated.addTerm(dark.get(), 0.5, fl::null);
        aggregated.addTerm(medium.get(), 0.1, fl::null);
        aggregated.addTerm(bright.get(), 0.6, fl::null);

        CHECK(aggregated.highestActivatedTerm().getTerm() == bright.get());

        aggregated.terms().at(1).setDegree(0.7);
        CHECK(aggregated.highestActivatedTerm().getTerm() == medium.get());

        aggregated.terms().front().setDegree(0.9);
        CHECK(aggregated.highestActivatedTerm().getTerm() == dark.get());

        aggregated.clear();
        CHECK(aggregated.maximallyActivatedTerms().empty());
    }

    TEST_CASE("Aggregated: grouped terms", "[term][aggregated]") {
        std::unique_ptr<Term> dark(new Triangle("DARK", 0.000, 0.250, 0.500));
        std::unique_ptr<Term> bright(new Triangle("BRIGHT", 0.500, 0.750, 1.000));
        std::unique_ptr<Term> medium(new Triangle("MED", 0.500, 0.750, 1.000));

        Aggregated aggregated;

        CHECK(aggregated.groupedTerms().empty());

        aggregated.addTerm(dark.get(), 0.5, fl::null);
        aggregated.addTerm(dark.get(), 0.5, fl::null);
        aggregated.addTerm(bright.get(), 0.25, fl::null);
        aggregated.addTerm(bright.get(), 0.25, fl::null);
        aggregated.addTerm(medium.get(), 0.1, fl::null);

        // Without aggregation
        auto grouped = aggregated.groupedTerms();
        CHECK(grouped.at(0).getTerm() == dark.get());
        CHECK(grouped.at(0).getDegree() == 1.0);

        CHECK(grouped.at(1).getTerm() == bright.get());
        CHECK(grouped.at(1).getDegree() == 0.5);

        CHECK(grouped.at(2).getTerm() == medium.get());
        CHECK(grouped.at(2).getDegree() == scalar(0.1));

        // With aggregation
        aggregated.setAggregation(new Maximum);
        grouped = aggregated.groupedTerms();
        CHECK(grouped.at(0).getTerm() == dark.get());
        CHECK(grouped.at(0).getDegree() == 0.5);

        CHECK(grouped.at(1).getTerm() == bright.get());
        CHECK(grouped.at(1).getDegree() == 0.25);

        CHECK(grouped.at(2).getTerm() == medium.get());
        CHECK(grouped.at(2).getDegree() == scalar(0.1));
    }

    TEST_CASE("Bell", "[term][bell]") {
        TermAssert<Bell>("bell")
            .can_clone()
            .can_construct()
            .takes_parameters(3)
            .exports_fll("term: bell Bell nan nan nan")
            .repr_is("fl.Bell('bell', fl.nan, fl.nan, fl.nan)")
            .is_not_monotonic();

        TermAssert<Bell>("bell")
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
        TermAssert<Binary>("binary")
            .can_clone()
            .can_construct()
            .exports_fll("term: binary Binary nan nan")
            .repr_is("fl.Binary('binary', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic()
            .apply([](Term* binary) -> void {
                CHECK(dynamic_cast<Binary*>(binary)->direction() == Binary::Direction::Undefined);
            });

        TermAssert<Binary>("binary")
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
            })
            .apply([](Term* binary) -> void {
                CHECK(dynamic_cast<Binary*>(binary)->direction() == Binary::Direction::Positive);
            });
        TermAssert<Binary>("binary")
            .configured_as("0 -inf")
            .exports_fll("term: binary Binary 0.000 -inf")
            .repr_is("fl.Binary('binary', 0.0, -fl.inf)")
            .has_memberships({
                {-1.0, 1.0},
                {-0.5, 1.0},
                {-0.4, 1.0},
                {-0.25, 1.0},
                {-0.1, 1.0},
                {0.0, 1.0},
                {0.1, 0.0},
                {0.25, 0.0},
                {0.4, 0.0},
                {0.5, 0.0},
                {1.0, 0.0},
                {fl::nan, fl::nan},
                {fl::inf, 0.0},
                {-fl::inf, 1.0},
            })
            .apply([](Term* binary) -> void {
                CHECK(dynamic_cast<Binary*>(binary)->direction() == Binary::Direction::Negative);
            });
    }

    TEST_CASE("Concave", "[term][concave]") {
        TermAssert<Concave>("concave")
            .can_clone()
            .can_construct()
            .exports_fll("term: concave Concave nan nan")
            .repr_is("fl.Concave('concave', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();

        TermAssert<Concave>("concave")
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

        TermAssert<Concave>("concave")
            .configured_as("0.00 -0.500 0.5")
            .repr_is("fl.Concave('concave', 0.0, -0.5, 0.5)")
            .exports_fll("term: concave Concave 0.000 -0.500 0.500");
    }

    TEST_CASE("Concave Tsukamoto", "[term][concave][tsukamoto]") {
        TermAssert<Concave>("concave")
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

        TermAssert<Concave>("concave")
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
        TermAssert<Constant>("constant")
            .can_clone()
            .can_construct()
            .exports_fll("term: constant Constant nan", false)
            .repr_is("fl.Constant('constant', fl.nan)")
            .takes_parameters(1, false)
            .is_not_monotonic();

        TermAssert<Constant>("constant")
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
            });
        TermAssert<Constant>("constant")
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
        TermAssert<Cosine>("cosine")
            .can_clone()
            .can_construct()
            .exports_fll("term: cosine Cosine nan nan")
            .repr_is("fl.Cosine('cosine', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic();

        TermAssert<Cosine>("cosine")
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
        TermAssert<Discrete>("discrete")
            .can_clone()
            .can_construct()
            .exports_fll("term: discrete Discrete")
            .repr_is("fl.Discrete('discrete')")
            .is_not_monotonic();

        TermAssert<Discrete>("discrete")
            .configured_as("0 1 8 9 4 5 2 3 6 7")
            .exports_fll("term: discrete Discrete 0.000 1.000 8.000 9.000 4.000 5.000 2.000 3.000 6.000 7.000")
            .apply([](Term* term) -> void { dynamic_cast<Discrete*>(term)->sort(); })
            .exports_fll("term: discrete Discrete 0.000 1.000 2.000 3.000 4.000 5.000 6.000 7.000 8.000 9.000")
            .repr_is("fl.Discrete('discrete', fl.array([fl.array([0.0, 1.0]), fl.array([2.0, "
                     "3.0]), fl.array([4.0, 5.0]), fl.array([6.0, 7.0]), fl.array([8.0, 9.0])]))");

        CHECK_THROWS_WITH(
            Discrete("discrete").membership(0), Catch::Matchers::StartsWith("[discrete error] term is empty")
        );

        TermAssert<Discrete>("discrete")
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

        TermAssert<Discrete>(Discrete::create("discrete", 4, 0.0, 1.0, 2.0, 3.0))
            .exports_fll("term: discrete Discrete 0.000 1.000 2.000 3.000");

        std::unique_ptr<Discrete> discrete(Discrete::create("discrete", 4, 0.0, 1.0, 2.0, 3.0));
        CHECK_THAT(discrete->x(), Catch::Matchers::Equals<scalar>({0.0, 2.0}));
        CHECK_THAT(discrete->y(), Catch::Matchers::Equals<scalar>({1.0, 3.0}));
        CHECK_THAT(discrete->x(1), Catch::Matchers::WithinAbs(2.0, fuzzylite::macheps()));
        CHECK_THAT(discrete->y(1), Catch::Matchers::WithinAbs(3.0, fuzzylite::macheps()));

        CHECK_THAT(Discrete::toVector(discrete->xy()), Catch::Matchers::Equals<scalar>({0.0, 1.0, 2.0, 3.0}));
        CHECK_THAT(Discrete::formatXY(discrete->xy()), Catch::Matchers::Equals("(0.000,1.000) (2.000,3.000)"));

        CHECK_THROWS_AS(Discrete::toPairs({0.0}), fl::Exception);
        CHECK_THROWS_WITH(
            Discrete::toPairs({0.0}),
            Catch::Matchers::StartsWith("[discrete error] missing value in set of pairs (|xy|=1)")
        );

        discrete->x(0) = -1.0;
        discrete->y(0) = -2.0;
        CHECK_THAT(discrete->x(0), Catch::Matchers::WithinAbs(-1.0, fuzzylite::macheps()));
        CHECK_THAT(discrete->y(0), Catch::Matchers::WithinAbs(-2.0, fuzzylite::macheps()));

        discrete->xy(1).first = 4.0;
        discrete->xy(1).second = 8.0;
        CHECK_THAT(discrete->x(1), Catch::Matchers::WithinAbs(4.0, fuzzylite::macheps()));
        CHECK_THAT(discrete->y(1), Catch::Matchers::WithinAbs(8.0, fuzzylite::macheps()));

        Triangle triangle("x", 0, 1);
        TermAssert<Discrete>(Discrete::discretize(&triangle, 0, 1, 5))
            .exports_fll("term: x Discrete 0.000 0.000 0.200 0.400 0.400 0.800 0.600 0.800 0.800 0.400 1.000 0.000");

        std::vector<Discrete::Pair> v = Discrete::toPairs({0.0}, 1.0);
        CHECK(v.front() == Discrete::Pair(0.0, 1.0));

        const Discrete term("const", {Discrete::Pair(0.0, 1.0)});
        CHECK(term.xy(0) == Discrete::Pair(0.0, 1.0));
        CHECK(term.x(0) == 0.0);
        CHECK(term.y(0) == 1.0);
    }

    TEST_CASE("discrete finds elements using binary search", "[term][discrete]") {
        fuzzylite::setLogging(true);
        fuzzylite::setDebugging(false);
        Rectangle rectangle("rectangle", 0, 1);
        std::unique_ptr<Discrete> discrete(
            Discrete::discretize(&rectangle, rectangle.getStart(), rectangle.getEnd(), 10)
        );
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
        std::unique_ptr<Discrete> discrete(
            Discrete::discretize(&triangle, triangle.getVertexA(), triangle.getVertexC(), 100)
        );
        FL_LOG(discrete->toString());
        for (int i = 0; i < 200; ++i) {
            scalar x = Op::scale(i, 0, 200, -1, 1);
            if (not Op::isEq(triangle.membership(x), discrete->membership(x))) {
                fuzzylite::setDebugging(true);
                CHECK(Op::isEq(triangle.membership(x), discrete->membership(x)));
                fuzzylite::setDebugging(false);
            }
        }
        fuzzylite::setLogging(false);
        fuzzylite::setDebugging(false);
    }

    TEST_CASE("discrete finds all elements using binary search", "[term][discrete]") {
        fuzzylite::setLogging(false);
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
        terms.push_back(
            new SigmoidDifference("sigmoidDifference", min + .25 * range, 20 / range, 20 / range, max - .25 * range)
        );
        terms.push_back(
            new SigmoidProduct("sigmoidProduct", min + .25 * range, 20 / range, 20 / range, max - .25 * range)
        );
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
                if (not Op::isEq(discrete.membership(x), term->membership(x)))
                    CHECK(discrete.membership(x) == term->membership(x));
            }
            for (int i = 0; i < 100; i++) {
                scalar x = Op::scale(i, 0, 100, -1, 1);
                if (not Op::isEq(discrete.membership(x), term->membership(x)))
                    CHECK(discrete.membership(x) == term->membership(x));
            }
        }
        for (std::size_t i = 0; i < terms.size(); ++i)
            delete terms.at(i);
    }

    TEST_CASE("Gaussian", "[term][gaussian]") {
        TermAssert<Gaussian>("gaussian")
            .can_clone()
            .can_construct()
            .exports_fll("term: gaussian Gaussian nan nan")
            .repr_is("fl.Gaussian('gaussian', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic();
        TermAssert<Gaussian>("gaussian")
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
        TermAssert<GaussianProduct>("gaussian_product")
            .can_clone()
            .can_construct()
            .exports_fll("term: gaussian_product GaussianProduct nan nan nan nan")
            .repr_is("fl.GaussianProduct('gaussian_product', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();
        TermAssert<GaussianProduct>("gaussian_product")
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

        linear = Linear("linear");
        linear.set({1.0, 2.0, 3.0}, &engine);
        CHECK_THAT(linear.coefficients(), Catch::Matchers::Equals<scalar>({1.0, 2.0, 3.0}));
        CHECK(linear.getEngine() == &engine);
        linear.coefficients().push_back(4.0);
        CHECK_THAT(linear.coefficients(), Catch::Matchers::Equals<scalar>({1.0, 2.0, 3.0, 4.0}));

        // TODO: Raise exception when mistmatch between coefficients and number of input variables
        // CHECK_THROWS_AS(linear.membership(0.0), fl::Exception);
        // CHECK_THROWS_WITH(
        //     linear.membership(0.0),
        //     Catch::Matchers::StartsWith(
        //         "expected 3 (+1) coefficients (one for each input variable plus an optional constant), "
        //         "but found 5 coefficients: [1.0, 2.0, 3.0, 5.0, 8.0]"
        //     )
        // );

        TermAssert<Linear>("linear", std::vector<scalar>{1.0, 2.0}, &engine)
            .can_clone()
            .can_construct()
            .exports_fll("term: linear Linear 1.000 2.000", false)
            .repr_is("fl.Linear('linear', [1.0, 2.0])")
            .is_not_monotonic()
            .configured_as("1.0 2.0 3")
            .exports_fll("term: linear Linear 1.000 2.000 3.000", false)
            .repr_is("fl.Linear('linear', [1.0, 2.0, 3.0])")
            .has_memberships({
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
            });
        TermAssert<Linear>("linear", std::vector<scalar>{1.0, 2.0}, &engine)
            .configured_as("1 2 3 5")
            .exports_fll("term: linear Linear 1.000 2.000 3.000 5.000", false)
            .has_memberships({
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
            });
    }

    TEST_CASE("PiShape", "[term][pishape]") {
        TermAssert<PiShape>("pi_shape")
            .can_clone()
            .can_construct()
            .exports_fll("term: pi_shape PiShape nan nan nan nan")
            .repr_is("fl.PiShape('pi_shape', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();

        TermAssert<PiShape>("pi_shape")
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
        TermAssert<Ramp>("ramp")
            .can_clone()
            .can_construct()
            .exports_fll("term: ramp Ramp nan nan")
            .repr_is("fl.Ramp('ramp', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic()
            .apply([](Term* ramp) -> void { CHECK(dynamic_cast<Ramp*>(ramp)->direction() == Ramp::Direction::Zero); });

        TermAssert<Ramp>("ramp")
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
            })
            .apply([](Term* ramp) -> void {
                CHECK(dynamic_cast<Ramp*>(ramp)->direction() == Ramp::Direction::Positive);
            });

        TermAssert<Ramp>("ramp")
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
            })
            .apply([](Term* ramp) -> void {
                CHECK(dynamic_cast<Ramp*>(ramp)->direction() == Ramp::Direction::Negative);
            });

        TermAssert<Ramp>("ramp")
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

        TermAssert<Ramp>("ramp")
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
        TermAssert<Ramp>("ramp")
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

        TermAssert<Ramp>("ramp")
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

        TermAssert<Ramp>("ramp")
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

    TEST_CASE("Ramp Tsukamoto Height", "[term][ramp][tsukamoto]") {
        TermAssert<Ramp>("ramp")
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
        TermAssert<Rectangle>("rectangle")
            .can_clone()
            .can_construct()
            .exports_fll("term: rectangle Rectangle nan nan")
            .repr_is("fl.Rectangle('rectangle', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic();
        TermAssert<Rectangle>("rectangle")
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
        TermAssert<Sigmoid>("sigmoid")
            .can_clone()
            .can_construct()
            .exports_fll("term: sigmoid Sigmoid nan nan")
            .repr_is("fl.Sigmoid('sigmoid', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic()
            .apply([](Term* sigmoid) -> void {
                CHECK(dynamic_cast<Sigmoid*>(sigmoid)->direction() == Sigmoid::Direction::Zero);
            });

        TermAssert<Sigmoid>("sigmoid")
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
            })
            .apply([](Term* sigmoid) -> void {
                CHECK(dynamic_cast<Sigmoid*>(sigmoid)->direction() == Sigmoid::Direction::Positive);
            });

        TermAssert<Sigmoid>("sigmoid")
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
            })
            .apply([](Term* sigmoid) -> void {
                CHECK(dynamic_cast<Sigmoid*>(sigmoid)->direction() == Sigmoid::Direction::Negative);
            });
    }

    TEST_CASE("Sigmoid Tsukamoto", "[term][sigmoid][tsukamoto]") {
        TermAssert<Sigmoid>("sigmoid")
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

        TermAssert<Sigmoid>("sigmoid")
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
        TermAssert<SigmoidDifference>("sigmoid_difference")
            .can_clone()
            .can_construct()
            .exports_fll("term: sigmoid_difference SigmoidDifference nan nan nan nan")
            .repr_is("fl.SigmoidDifference('sigmoid_difference', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();

        TermAssert<SigmoidDifference>("sigmoid_difference")
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
        TermAssert<SigmoidProduct>("sigmoid_product")
            .can_clone()
            .can_construct()
            .exports_fll("term: sigmoid_product SigmoidProduct nan nan nan nan")
            .repr_is("fl.SigmoidProduct('sigmoid_product', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();
        TermAssert<SigmoidProduct>("sigmoid_product")
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
        TermAssert<Spike>("spike")
            .can_clone()
            .can_construct()
            .exports_fll("term: spike Spike nan nan")
            .repr_is("fl.Spike('spike', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_not_monotonic();
        TermAssert<Spike>("spike")
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
        TermAssert<SShape>("s_shape")
            .can_clone()
            .can_construct()
            .exports_fll("term: s_shape SShape nan nan")
            .repr_is("fl.SShape('s_shape', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();
        TermAssert<SShape>("s_shape")
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

    TEST_CASE("SShape Tsukamoto", "[term][sshape][tsukamoto]") {
        TermAssert<SShape>("s_shape")
            .exports_fll("term: s_shape SShape nan nan")
            .repr_is("fl.SShape('s_shape', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();
        TermAssert<SShape>("s_shape")
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

        TermAssert<SShape>("s_shape")
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
        TermAssert<Trapezoid>("trapezoid")
            .can_clone()
            .can_construct()
            .exports_fll("term: trapezoid Trapezoid nan nan nan nan")
            .repr_is("Trapezoid('trapezoid', fl.nan, fl.nan, fl.nan, fl.nan)")
            .takes_parameters(4)
            .is_not_monotonic();

        TermAssert<Trapezoid>("trapezoid", 0.0, 1.0)
            .exports_fll("term: trapezoid Trapezoid 0.000 0.200 0.800 1.000")
            .repr_is("Trapezoid('trapezoid', 0.0, 0.2, 0.8, 1.0)");

        TermAssert<Trapezoid>("trapezoid")
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
        TermAssert<Trapezoid>("trapezoid")
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
        TermAssert<Trapezoid>("trapezoid")
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
        TermAssert<Trapezoid>("trapezoid")
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
        TermAssert<Trapezoid>("trapezoid")
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
        TermAssert<Triangle>("triangle")
            .can_clone()
            .can_construct()
            .exports_fll("term: triangle Triangle nan nan nan")
            .repr_is("fl.Triangle('triangle', fl.nan, fl.nan, fl.nan)")
            .takes_parameters(3)
            .is_not_monotonic();

        TermAssert<Triangle>("triangle", 0.0, 1.0)
            .exports_fll("term: triangle Triangle 0.000 0.500 1.000")
            .repr_is("fl.Triangle('triangle', 0.0, 0.5, 1.0)");

        TermAssert<Triangle>("triangle")
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
        TermAssert<Triangle>("triangle")
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
        TermAssert<Triangle>("triangle")
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
        TermAssert<Triangle>("triangle")
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
        TermAssert<Triangle>("triangle")
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
        TermAssert<Triangle>("triangle")
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
        TermAssert<Triangle>("triangle")
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
        TermAssert<ZShape>("z_shape")
            .can_clone()
            .can_construct()
            .exports_fll("term: z_shape ZShape nan nan")
            .repr_is("fl.ZShape('z_shape', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();

        TermAssert<ZShape>("z_shape")
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

        TermAssert<ZShape>("z_shape")
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

    TEST_CASE("ZShape Tsukamoto", "[term][zshape][tsukamoto]") {
        TermAssert<ZShape>("z_shape")
            .exports_fll("term: z_shape ZShape nan nan")
            .repr_is("fl.ZShape('z_shape', fl.nan, fl.nan)")
            .takes_parameters(2)
            .is_monotonic();

        TermAssert<ZShape>("z_shape")
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

        TermAssert<ZShape>("z_shape")
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

    TEST_CASE("Function", "[term][function]") {
        TermAssert<Function>("f", "ge(x, 5)", std::map<std::string, scalar>{}, fl::null, true)  //
            .can_clone()
            .can_construct()
            .has_memberships({{4.0, 0.0}, {5.0, 1.0}}, {1.0});

        Function f("f", "ge(x, 5)");
        CHECK(not f.isLoaded());
        f.load();
        CHECK(f.isLoaded());
        f.unload();
        CHECK(not f.isLoaded());
        CHECK_THROWS_AS(f.membership(0.0), fl::Exception);
        CHECK_THROWS_WITH(
            f.membership(0.0), Catch::Matchers::StartsWith("[function error] function <ge(x, 5)> not loaded")
        );

        CHECK_THROWS_AS(f.evaluate(), fl::Exception);
        CHECK_THROWS_WITH(f.evaluate(), Catch::Matchers::StartsWith("[function error] function <ge(x, 5)> not loaded"));

        f.configure("1 / (x-5)");
        CHECK(f.membership(5) == fl::inf);
        CHECK(f.membership(4) == -1.0);

        f.configure("0 / (x-5)");
        CHECK(Op::isNaN(f.membership(5)));
        CHECK(f.membership(4) == 0.0);

        CHECK(std::unique_ptr<Function>(Function::create("f", "3.000 ^ 4.000"))->evaluate() == 81.0);
        CHECK_THAT(
            std::unique_ptr<Function>(Function::create("f", "sin ( 3.000 ^ 4.000 )"))->evaluate(),
            Catch::Matchers::WithinAbs(-0.629887994274454, fuzzylite::macheps())
        );
        CHECK_THAT(
            Function("f", "pow ( sin ( 3.000 ^ 4.000 ) two )", {{"two", 2}}, fl::null, true).evaluate(),
            Catch::Matchers::WithinAbs(0.39675888533109455, fuzzylite::macheps())
        );
        CHECK_THAT(
            Function(
                "f",
                "pow ( sin ( 3.000 ^ 4.000 ) two ) + pow ( sin ( 3.000 ^ 4.000 ) two )",
                {{"two", 2}},
                fl::null,
                true
            )
                .evaluate(),
            Catch::Matchers::WithinAbs(0.7935177706621891, fuzzylite::macheps())
        );
        CHECK_THAT(
            Function("f", "cos atan(1) * 4", {}, fl::null, true).evaluate(),
            Catch::Matchers::WithinAbs(-1.0, fuzzylite::macheps())
        );

        TermAssert<Function>("f", "", std::map<std::string, scalar>{{"y", 1.5}})
            .configured_as("2*x^3 +2*y - 3", false)
            .exports_fll("term: f Function 2*x^3 +2*y - 3", false)
            .has_memberships(
                {
                    {-0.5, -0.25},
                    {-0.4, -0.1280000000000001},
                    {-0.25, -0.03125},
                    {-0.1, -0.0019999999999997797},
                    {0.0, 0.0},
                    {0.1, 0.0019999999999997797},
                    {0.25, 0.03125},
                    {0.4, 0.1280000000000001},
                    {0.5, 0.25},
                    {nan, nan},
                    {inf, inf},
                    {-inf, -inf},
                },
                {1.0}
            );

        CHECK_THROWS_AS(Function::create("f", "2*i_A + o_A + x"), fl::Exception);
        CHECK_THROWS_WITH(
            Function::create("f", "2*i_A + o_A + x"), Catch::Matchers::StartsWith("[function error] unknown variable")
        );

        Engine engine("A", "Engine A", {new InputVariable("i_A")}, {new OutputVariable("o_A")});
        TermAssert<Function>(Function::create("f", "2*i_A + o_A + x", &engine))
            .can_clone()
            .exports_fll("term: f Function 2*i_A + o_A + x", false)
            .repr_is("fl.Function('f', '2*i_A + o_A + x')")
            .has_memberships({{0.0, nan}, {nan, nan}}, {1.0});

        engine.getInputVariable("i_A")->setValue(3.0);
        engine.getOutputVariable("o_A")->setValue(1.0);
        TermAssert<Function>(Function::create("f", "2*i_A + o_A + x", &engine))
            .exports_fll("term: f Function 2*i_A + o_A + x", false)
            .repr_is("fl.Function('f', '2*i_A + o_A + x')")
            .has_memberships(
                {
                    {-1.0, 6.0},
                    {-0.5, 6.5},
                    {0.0, 7.0},
                    {0.5, 7.5},
                    {1.0, 8.0},
                    {nan, nan},
                    {inf, inf},
                    {-inf, -inf},
                },
                {1.0}
            );

        const Function constFunction("f", "x", fl::null);
        f = constFunction;
        CHECK(f.toString() == constFunction.toString());
    }

    TEST_CASE("FunctionElementOperator", "[term][function][element]") {
        SECTION("Base") {
            auto op = Function::Element("Name", "Description", Function::Element::Operator);
            CHECK(
                op.toString()
                == "Operator (name=Name, description=Description, precedence=0, arity=0, associativity=-1, "
                   "pointer=arity(0))"
            );
            CHECK(op.isOperator());
            CHECK(not op.isFunction());
            CHECK(std::unique_ptr<Function::Element>(op.clone())->toString() == op.toString());
        }
        SECTION("Unary") {
            auto op = Function::Element("~", "Negation", Function::Element::Operator, &(Op::negate), 0, 1);
            CHECK(
                op.toString()
                == "Operator (name=~, description=Negation, precedence=0, arity=1, associativity=1, pointer=unary)"
            );
            CHECK(op.isOperator());
            CHECK(not op.isFunction());
            CHECK(std::unique_ptr<Function::Element>(op.clone())->toString() == op.toString());
        }
        SECTION("Binary") {
            auto op = Function::Element("*", "Multiplication", Function::Element::Operator, &(Op::multiply), 10);
            CHECK(
                op.toString()
                == "Operator (name=*, description=Multiplication, precedence=10, arity=2, associativity=-1, "
                   "pointer=binary)"
            );
            CHECK(op.isOperator());
            CHECK(not op.isFunction());
            CHECK(std::unique_ptr<Function::Element>(op.clone())->toString() == op.toString());
        }
    }

    TEST_CASE("FunctionElementFunction", "[term][function][element]") {
        SECTION("Base") {
            auto f = Function::Element("Name", "Description", Function::Element::Function);
            CHECK(
                f.toString()
                == "Function (name=Name, description=Description, arity=0, associativity=-1, pointer=arity(0))"
            );
            CHECK(f.isFunction());
            CHECK(not f.isOperator());
            CHECK(std::unique_ptr<Function::Element>(f.clone())->toString() == f.toString());

            CHECK_THROWS_AS(Function::Node(f.clone()).evaluate(), fl::Exception);
            CHECK_THROWS_WITH(
                Function::Node(f.clone()).evaluate(),
                Catch::Matchers::StartsWith(
                    "[function error] expected a pointer to a unary or binary function in node <Name>, but got none"
                )
            );
        }
        SECTION("Unary") {
            auto f = Function::Element("cos", "Cosine", Function::Element::Function, &(std::cos));
            CHECK(f.toString() == "Function (name=cos, description=Cosine, arity=1, associativity=-1, pointer=unary)");
            CHECK(f.isFunction());
            CHECK(not f.isOperator());
            CHECK(std::unique_ptr<Function::Element>(f.clone())->toString() == f.toString());

            auto node = Function::Node(f.clone());
            CHECK_THROWS_AS(node.evaluate(), fl::Exception);
            CHECK_THROWS_WITH(
                node.evaluate(), Catch::Matchers::StartsWith("[function error] expected one node, but got none")
            );
            // Left == Right
            CHECK(
                Function::Node(f.clone(), new Function::Node(0.0)).evaluate()
                == Function::Node(f.clone(), fl::null, new Function::Node(0.0)).evaluate()
            );
        }
        SECTION("Binary") {
            auto f = Function::Element("gt", "Greater than (>)", Function::Element::Function, &(Op::gt));
            CHECK(
                f.toString()
                == "Function (name=gt, description=Greater than (>), arity=2, associativity=-1, pointer=binary)"
            );
            CHECK(f.isFunction());
            CHECK(not f.isOperator());
            CHECK(std::unique_ptr<Function::Element>(f.clone())->toString() == f.toString());

            auto node = Function::Node(f.clone());
            CHECK_THROWS_AS(node.evaluate(), fl::Exception);
            CHECK_THROWS_WITH(
                node.evaluate(), Catch::Matchers::StartsWith("[function error] expected two nodes, but got fewer")
            );
            node.left.reset(new Function::Node(0.0));
            CHECK_THROWS_AS(node.evaluate(), fl::Exception);
            CHECK_THROWS_WITH(
                node.evaluate(), Catch::Matchers::StartsWith("[function error] expected two nodes, but got fewer")
            );
            node.left.reset();

            node.right.reset(new Function::Node(0.0));
            CHECK_THROWS_AS(node.evaluate(), fl::Exception);
            CHECK_THROWS_WITH(
                node.evaluate(), Catch::Matchers::StartsWith("[function error] expected two nodes, but got fewer")
            );
            node.right.reset();

            node.left.reset(new Function::Node(1.0));
            node.right.reset(new Function::Node(0.0));
            CHECK(node.evaluate() == 1.0);
        }
    }

    struct FunctionNodeAssert {
        Function::Node* actual;

        FunctionNodeAssert(Function::Node* actual) : actual(actual) {}

        FunctionNodeAssert& prefix_is(const std::string& prefix) {
            CHECK(actual->toPrefix() == prefix);
            return *this;
        }

        FunctionNodeAssert& infix_is(const std::string& infix) {
            CHECK(actual->toInfix() == infix);
            return *this;
        }

        FunctionNodeAssert& postfix_is(const std::string& postfix) {
            CHECK(actual->toPostfix() == postfix);
            return *this;
        }

        FunctionNodeAssert& value_is(const std::string& expected) {
            CHECK(actual->value() == expected);
            return *this;
        }

        FunctionNodeAssert& evaluates_to(scalar expected, const std::map<std::string, scalar>& variables = {}) {
            const scalar obtained = actual->evaluate(&variables);
            CHECK_THAT(obtained, Catch::Matchers::WithinAbs(expected, fuzzylite::macheps()));
            return *this;
        }

        FunctionNodeAssert& fails_to_evaluate(const std::string& message) {
            CHECK_THROWS_AS(actual->evaluate(), fl::Exception);
            CHECK_THROWS_WITH(actual->evaluate(), Catch::Matchers::StartsWith(message));
            return *this;
        }
    };

    TEST_CASE("FunctionNode", "[term][function][element]") {
        const FunctionFactory ff;
        std::unique_ptr<Function::Node> node_pow(
            new Function::Node(ff.cloneObject("^"), new Function::Node(3.0), new Function::Node(4.0))
        );
        FunctionNodeAssert(node_pow->clone())
            .postfix_is("3.000 4.000 ^")
            .prefix_is("^ 3.000 4.000")
            .infix_is("3.000 ^ 4.000")
            .evaluates_to(81.0);

        std::unique_ptr<Function::Node> node_sin(new Function::Node(ff.cloneObject("sin"), fl::null, node_pow->clone())
        );
        FunctionNodeAssert(node_sin->clone())
            .postfix_is("3.000 4.000 ^ sin")
            .prefix_is("sin ^ 3.000 4.000")
            .infix_is("sin ( 3.000 ^ 4.000 )")
            .evaluates_to(-0.629887994274454);

        node_pow.reset(new Function::Node(ff.cloneObject("pow"), node_sin->clone(), new Function::Node("two")));
        FunctionNodeAssert(node_pow->clone())
            .postfix_is("3.000 4.000 ^ sin two pow")
            .prefix_is("pow sin ^ 3.000 4.000 two")
            .infix_is("pow ( sin ( 3.000 ^ 4.000 ) two )")
            .fails_to_evaluate("[function error] expected a map of variables containing the value for 'two', "
                               "but none was provided")
            .evaluates_to(0.39675888533109455, {{"two", 2}});

        std::unique_ptr<Function::Node> node_sum(
            new Function::Node(ff.cloneObject("+"), node_pow->clone(), node_pow->clone())
        );
        FunctionNodeAssert(node_sum->clone())
            .postfix_is("3.000 4.000 ^ sin two pow 3.000 4.000 ^ sin two pow +")
            .prefix_is("+ pow sin ^ 3.000 4.000 two pow sin ^ 3.000 4.000 two")
            .infix_is("pow ( sin ( 3.000 ^ 4.000 ) two ) + pow ( sin ( 3.000 ^ 4.000 ) two )")
            .evaluates_to(0.7935177706621891, {{"two", 2}});
    }

    TEST_CASE("FunctionNodeValue", "[term][function][element]") {
        const FunctionFactory ff;
        CHECK(Function::Node(ff.cloneObject("+")).toString() == "+");
        CHECK(Function::Node("variable").toString() == "variable");
        CHECK(Function::Node(fl::nan).toString() == "nan");

        const auto node = Function::Node("variable");
        Function::Node copy("x");
        copy = node;
        CHECK(node.toString() == copy.toString());
    }

    TEST_CASE("FunctionNodeNegative", "[term][function][element]") {
        const FunctionFactory ff;
        FunctionNodeAssert(new Function::Node(ff.cloneObject("~"), new Function::Node(5)))
            .infix_is("~ 5.000")
            .postfix_is("5.000 ~")
            .prefix_is("~ 5.000")
            .evaluates_to(-5.0);

        auto x = new Function::Node(ff.cloneObject("*"));
        CHECK(x->toString() == "*");

        FunctionNodeAssert(x).fails_to_evaluate("");
    }

    TEST_CASE("Function parsing", "[term][function]") {
        SECTION("Arithmetic") {
            Function f;
            std::string text = "3+4*2/(1-5)^2^3";
            CHECK(f.toPostfix(text) == "3 4 2 * 1 5 - 2 3 ^ ^ / +");
            CHECK(f.parse(text)->toInfix() == "3.000 + 4.000 * 2.000 / 1.000 - 5.000 ^ 2.000 ^ 3.000");
            CHECK(f.parse(text)->toPrefix() == "+ 3.000 / * 4.000 2.000 ^ - 1.000 5.000 ^ 2.000 3.000");
        }

        SECTION("Function") {
            CHECK(Function().parse("3 * 4 * ge(1, 0)")->evaluate() == 12);
            CHECK(Function().parse("3 * 4 * ge(0, 1)")->evaluate() == 0);

            CHECK_THROWS_AS(Function().parse("3 * 4 * ge(0, 1"), fl::Exception);
            CHECK_THROWS_WITH(
                Function().parse("3 * 4 * ge(0, 1"),
                Catch::Matchers::StartsWith("[parsing error] mismatching parentheses in:")
            );

            CHECK_THROWS_AS(Function().parse("3 * 4 * ge(0, 1"), fl::Exception);
            CHECK_THROWS_WITH(
                Function().parse("3 * 4 * ge(0, 1"),
                Catch::Matchers::StartsWith("[parsing error] mismatching parentheses in:")
            );

            CHECK(Function().parse("3 * 4 * ge(0, le(0, 1))")->evaluate() == 0);
            CHECK_THROWS_AS(Function().parse("\"3 * 4 * ge(0, le(0, 1)"), fl::Exception);
            CHECK_THROWS_WITH(
                Function().parse("\"3 * 4 * ge(0, le(0, 1)"),
                Catch::Matchers::StartsWith("[parsing error] mismatching parentheses in:")
            );
        }

        SECTION("Trigonometry") {
            Function f;
            std::string text = "sin(y*x)^2/x";

            CHECK_THROWS(f.load(text));

            f.variables["y"] = 1.0;
            f.load(text);

            CHECK(f.toPostfix(text) == "y x * sin 2 ^ x /");
            CHECK(f.parse(text)->toInfix() == "sin ( y * x ) ^ 2.000 / x");
            CHECK(f.parse(text)->toPrefix() == "/ ^ sin * y x 2.000 x");
        }

        SECTION("Propositions") {
            std::string text = "(Temperature is High and Oxygen is Low) or "
                               "(Temperature is Low and (Oxygen is Low or Oxygen is High))";

            CHECK(
                Function().toPostfix(text)
                == "Temperature is High Oxygen is Low "
                   "and Temperature is Low Oxygen is Low Oxygen is High or and or"
            );
        }
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
        CHECK_THAT(f.parse(text)->evaluate(&f.variables), Approximates(20));

        f.load(text);

        f.variables["pi"] = 3.14;

        CHECK(f.toPostfix(text) == "5 ~ 4 * pi ~ 2 / sin /");
        CHECK(f.parse(text)->toInfix() == "~ 5.000 * 4.000 / sin ( ~ pi / 2.000 )");
        CHECK(f.parse(text)->toPrefix() == "/ * ~ 5.000 4.000 sin / ~ pi 2.000");
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

    TEST_CASE("Function computes tree size correctly", "[term][function]") {
        Function f("f", "x*x+(x-x)/x+log(x)");
        f.load();
        CHECK(f.root()->treeSize() == 6);
        CHECK(f.root()->treeSize(Function::Element::Function) == 1);
        CHECK(f.root()->treeSize(Function::Element::Operator) == 5);
    }

}}
