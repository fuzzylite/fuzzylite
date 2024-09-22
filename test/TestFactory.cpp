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
    template <typename T>
    struct ConstructionFactoryAssert {
        using Constructor = std::tuple<std::string, typename ConstructionFactory<T*>::Constructor, std::string>;
        FL_unique_ptr<ConstructionFactory<T*>> actual;

        explicit ConstructionFactoryAssert(ConstructionFactory<T*>* actual) : actual(actual) {}

        ConstructionFactoryAssert& has_class_name(const std::string& name) {
            CHECK(actual->name() == name);
            return *this;
        }

        ConstructionFactoryAssert& contains(const std::vector<std::string>& names, bool contains = true) {
            CAPTURE(names, contains);
            for (const std::string& name : names)
                CHECK(actual->hasConstructor(name) == contains);
            return *this;
        }

        ConstructionFactoryAssert&
        constructs_exactly(const std::map<std::string, typename ConstructionFactory<T*>::Constructor>& constructors) {
            std::vector<std::string> expected;
            for (const auto& name_value : constructors) {
                CAPTURE(name_value.first);
                expected.push_back(name_value.first);
                CHECK(actual->hasConstructor(name_value.first));
                CHECK(actual->getConstructor(name_value.first) == name_value.second);
                std::unique_ptr<T> object(actual->constructObject(name_value.first));
                if (name_value.first.empty())
                    CHECK(object.get() == fl::null);
                else
                    CHECK_THAT(fl::FllExporter().toString(object.get()), Catch::Matchers::StartsWith(name_value.first));
            }
            CHECK(actual->available() == expected);
            return *this;
        }

        ConstructionFactoryAssert& constructs_exactly(const std::vector<Constructor>& constructors) {
            std::vector<std::string> expected;
            for (const auto& name_constructor_fll : constructors) {
                std::string name;
                typename ConstructionFactory<T*>::Constructor constructor;
                std::string fll;
                std::tie(name, constructor, fll) = name_constructor_fll;
                CAPTURE(name, fll);
                expected.push_back(name);
                CHECK(actual->hasConstructor(name));
                CHECK(actual->getConstructor(name) == constructor);
                std::unique_ptr<T> object(actual->constructObject(name));
                if (name.empty())
                    CHECK(object.get() == fl::null);
                else
                    CHECK_THAT(fl::FllExporter().toString(object.get()), Catch::Matchers::Equals(fll));
            }
            CHECK_THAT(actual->available(), Catch::Matchers::UnorderedEquals(expected));
            return *this;
        }

        ConstructionFactoryAssert& deregister_all() {
            for (const auto& constructor : actual->available()) {
                CHECK(actual->hasConstructor(constructor));
                actual->deregisterConstructor(constructor);
                CHECK(not actual->hasConstructor(constructor));
            }
            CHECK(actual->constructors().empty());
            return *this;
        }
    };

    struct ActivationFactoryAssert : ConstructionFactoryAssert<Activation> {
        explicit ActivationFactoryAssert(ActivationFactory* actual) : ConstructionFactoryAssert(actual) {}
    };

    struct DefuzzifierFactoryAssert : ConstructionFactoryAssert<Defuzzifier> {
        explicit DefuzzifierFactoryAssert(DefuzzifierFactory* actual) : ConstructionFactoryAssert(actual) {}
    };

    struct HedgeFactoryAssert : ConstructionFactoryAssert<Hedge> {
        explicit HedgeFactoryAssert(HedgeFactory* actual) : ConstructionFactoryAssert(actual) {}
    };

    struct SNormFactoryAssert : ConstructionFactoryAssert<SNorm> {
        explicit SNormFactoryAssert(SNormFactory* actual) : ConstructionFactoryAssert(actual) {}
    };

    struct TNormFactoryAssert : ConstructionFactoryAssert<TNorm> {
        explicit TNormFactoryAssert(TNormFactory* actual) : ConstructionFactoryAssert(actual) {}
    };

    struct TermFactoryAssert : ConstructionFactoryAssert<Term> {
        explicit TermFactoryAssert(TermFactory* actual) : ConstructionFactoryAssert(actual) {}
    };

    TEST_CASE("ActivationFactory", "[factory][activation]") {
        SECTION("Default name") {
            ActivationFactoryAssert(new ActivationFactory).has_class_name("Activation");
        }
        SECTION("Contains") {
            ActivationFactoryAssert(new ActivationFactory).contains({"First", "Last", "Threshold"});
        }
        SECTION("Does not contain") {
            ActivationFactoryAssert(new ActivationFactory).contains({"Second", "Third"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<ConstructionFactoryAssert<Activation>::Constructor> constructs{
                {"", fl::null, ""},
                {"First", First::constructor, "First 1 0.000"},
                {"General", General::constructor, "General"},
                {"Highest", Highest::constructor, "Highest 1"},
                {"Last", Last::constructor, "Last 1 0.000"},
                {"Lowest", Lowest::constructor, "Lowest 1"},
                {"Proportional", Proportional::constructor, "Proportional"},
                {"Threshold", Threshold::constructor, "Threshold >= 0.000"},
            };
            ActivationFactoryAssert(new ActivationFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            ActivationFactoryAssert(new ActivationFactory).deregister_all();
        }
    }

    TEST_CASE("DefuzzifierFactory", "[factory][defuzzifier]") {
        SECTION("Default name") {
            DefuzzifierFactoryAssert(new DefuzzifierFactory).has_class_name("Defuzzifier");
        }
        SECTION("Contains") {
            DefuzzifierFactoryAssert(new DefuzzifierFactory).contains({"Bisector", "Centroid", "WeightedAverage"});
        }
        SECTION("Does not contain") {
            DefuzzifierFactoryAssert(new DefuzzifierFactory).contains({"CenterOfGravity", "CoG"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<ConstructionFactoryAssert<Defuzzifier>::Constructor> constructs{
                {"", fl::null, ""},
                {"Bisector", Bisector::constructor, "Bisector"},
                {"Centroid", Centroid::constructor, "Centroid"},
                {"LargestOfMaximum", LargestOfMaximum::constructor, "LargestOfMaximum"},
                {"MeanOfMaximum", MeanOfMaximum::constructor, "MeanOfMaximum"},
                {"SmallestOfMaximum", SmallestOfMaximum::constructor, "SmallestOfMaximum"},
                {"WeightedAverage", WeightedAverage::constructor, "WeightedAverage"},
                {"WeightedSum", WeightedSum::constructor, "WeightedSum"},
            };
            DefuzzifierFactoryAssert(new DefuzzifierFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            DefuzzifierFactoryAssert(new DefuzzifierFactory).deregister_all();
        }
    }

    TEST_CASE("HedgeFactory", "[factory][hedge]") {
        SECTION("Default name") {
            HedgeFactoryAssert(new HedgeFactory).has_class_name("Hedge");
        }
        SECTION("Contains") {
            HedgeFactoryAssert(new HedgeFactory).contains({"very", "extremely", "not"});
        }
        SECTION("Does not contain") {
            HedgeFactoryAssert(new HedgeFactory).contains({"way", "often"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<ConstructionFactoryAssert<Hedge>::Constructor> constructs{
                {"", fl::null, ""},
                {"any", Any::constructor, "any"},
                {"extremely", Extremely::constructor, "extremely"},
                {"not", Not::constructor, "not"},
                {"seldom", Seldom::constructor, "seldom"},
                {"somewhat", Somewhat::constructor, "somewhat"},
                {"very", Very::constructor, "very"},
            };
            HedgeFactoryAssert(new HedgeFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            HedgeFactoryAssert(new HedgeFactory).deregister_all();
        }
    }

    TEST_CASE("SNormFactory", "[factory][snorm]") {
        SECTION("Default name") {
            SNormFactoryAssert(new SNormFactory).has_class_name("SNorm");
        }
        SECTION("Contains") {
            SNormFactoryAssert(new SNormFactory).contains({"AlgebraicSum", "EinsteinSum", "Maximum"});
        }
        SECTION("Does not contain") {
            SNormFactoryAssert(new SNormFactory).contains({"AlgebraicProduct", "EinsteinProduct"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<ConstructionFactoryAssert<SNorm>::Constructor> constructs{
                {"", fl::null, ""},
                {"AlgebraicSum", AlgebraicSum::constructor, "AlgebraicSum"},
                {"BoundedSum", BoundedSum::constructor, "BoundedSum"},
                {"DrasticSum", DrasticSum::constructor, "DrasticSum"},
                {"EinsteinSum", EinsteinSum::constructor, "EinsteinSum"},
                {"HamacherSum", HamacherSum::constructor, "HamacherSum"},
                {"Maximum", Maximum::constructor, "Maximum"},
                {"NilpotentMaximum", NilpotentMaximum::constructor, "NilpotentMaximum"},
                {"NormalizedSum", NormalizedSum::constructor, "NormalizedSum"},
                {"UnboundedSum", UnboundedSum::constructor, "UnboundedSum"},
            };
            SNormFactoryAssert(new SNormFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            SNormFactoryAssert(new SNormFactory).deregister_all();
        }
    }

    TEST_CASE("TNormFactory", "[factory][tnorm]") {
        SECTION("Default name") {
            TNormFactoryAssert(new TNormFactory).has_class_name("TNorm");
        }
        SECTION("Contains") {
            TNormFactoryAssert(new TNormFactory).contains({"AlgebraicProduct", "EinsteinProduct", "Minimum"});
        }
        SECTION("Does not contain") {
            TNormFactoryAssert(new TNormFactory).contains({"AlgebraicSum", "EinsteinSum"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<ConstructionFactoryAssert<TNorm>::Constructor> constructs{
                {"", fl::null, ""},
                {"AlgebraicProduct", AlgebraicProduct::constructor, "AlgebraicProduct"},
                {"BoundedDifference", BoundedDifference::constructor, "BoundedDifference"},
                {"DrasticProduct", DrasticProduct::constructor, "DrasticProduct"},
                {"EinsteinProduct", EinsteinProduct::constructor, "EinsteinProduct"},
                {"HamacherProduct", HamacherProduct::constructor, "HamacherProduct"},
                {"Minimum", Minimum::constructor, "Minimum"},
                {"NilpotentMinimum", NilpotentMinimum::constructor, "NilpotentMinimum"},
            };
            TNormFactoryAssert(new TNormFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            TNormFactoryAssert(new TNormFactory).deregister_all();
        }
    }

    TEST_CASE("TermFactory", "[factory][term]") {
        SECTION("Default name") {
            TermFactoryAssert(new TermFactory).has_class_name("Term");
        }
        SECTION("Contains") {
            TermFactoryAssert(new TermFactory).contains({"Constant", "Function", "Triangle"});
        }
        SECTION("Does not contain") {
            TermFactoryAssert(new TermFactory).contains({"Arc", "SemiEllipse"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<ConstructionFactoryAssert<Term>::Constructor> constructs{
                {"", fl::null, ""},
                // {"Arc", Arc::constructor, "term: _ Arc nan nan"},
                {"Bell", Bell::constructor, "term: _ Bell nan nan nan"},
                {"Binary", Binary::constructor, "term: _ Binary nan nan"},
                {"Concave", Concave::constructor, "term: _ Concave nan nan"},
                {"Constant", Constant::constructor, "term: _ Constant nan"},
                {"Cosine", Cosine::constructor, "term: _ Cosine nan nan"},
                {"Discrete", Discrete::constructor, "term: _ Discrete"},
                {"Function", Function::constructor, "term: _ Function"},
                {"Gaussian", Gaussian::constructor, "term: _ Gaussian nan nan"},
                {"GaussianProduct", GaussianProduct::constructor, "term: _ GaussianProduct nan nan nan nan"},
                {"Linear", Linear::constructor, "term: _ Linear"},
                {"PiShape", PiShape::constructor, "term: _ PiShape nan nan nan nan"},
                {"Ramp", Ramp::constructor, "term: _ Ramp nan nan"},
                {"Rectangle", Rectangle::constructor, "term: _ Rectangle nan nan"},
                // {"SemiEllipse", SemiEllipse::constructor, "term: _ SemiEllipse nan nan"},
                {"Sigmoid", Sigmoid::constructor, "term: _ Sigmoid nan nan"},
                {"SigmoidDifference", SigmoidDifference::constructor, "term: _ SigmoidDifference nan nan nan nan"},
                {"SigmoidProduct", SigmoidProduct::constructor, "term: _ SigmoidProduct nan nan nan nan"},
                {"Spike", Spike::constructor, "term: _ Spike nan nan"},
                {"SShape", SShape::constructor, "term: _ SShape nan nan"},
                {"Trapezoid", Trapezoid::constructor, "term: _ Trapezoid nan nan nan nan"},
                {"Triangle", Triangle::constructor, "term: _ Triangle nan nan nan"},
                {"ZShape", ZShape::constructor, "term: _ ZShape nan nan"},

            };
            TermFactoryAssert(new TermFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            TermFactoryAssert(new TermFactory).deregister_all();
        }
    }
}
