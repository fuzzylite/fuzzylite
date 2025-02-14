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
    struct ConstructionFactoryAssert {
        using Constructor = std::tuple<std::string, typename ConstructionFactory<T*>::Constructor, std::string>;
        std::unique_ptr<ConstructionFactory<T*>> actual;

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
                CAPTURE(constructor);
                CHECK(actual->hasConstructor(constructor));
                actual->deregisterConstructor(constructor);
                CHECK(not actual->hasConstructor(constructor));
            }
            CHECK(actual->constructors().empty());
            return *this;
        }

        ConstructionFactoryAssert& clones() {
            std::unique_ptr<ConstructionFactory<T*>> clone(actual->clone());
            CAPTURE(actual->name(), clone->name());
            CHECK(actual->name() == clone->name());
            std::vector<Constructor> constructors;
            for (auto key : actual->constructors()) {
                CAPTURE(key.first);
                std::unique_ptr<T> object(actual->constructObject(key.first));
                const std::string fll = object.get() ? fl::FllExporter().toString(object.get()) : "";
                constructors.push_back(std::make_tuple(key.first, key.second, fll));
            }
            this->constructs_exactly(constructors);
            return *this;
        }
    };

    TEST_CASE("ConstructionFactory", "[factory]") {
        SECTION("Unregistered constructor returns null") {
            ConstructionFactory<std::string*> cf("strings");
            CHECK(cf.getConstructor("X") == fl::null);
        }
        SECTION("Construct unregistered object raises exception") {
            const std::string expected = "[factory error] constructor of strings <X> not registered";
            ConstructionFactory<std::string*> cf("strings");
            CHECK_THROWS_AS(cf.constructObject("X"), fl::Exception);
            CHECK_THROWS_WITH(cf.constructObject("X"), Catch::Matchers::StartsWith(expected));
        }

        SECTION("Constructors vector") {
            ConstructionFactory<Term*> cf("terms");
            CHECK(cf.constructors().empty());
            cf.registerConstructor("triangle", Triangle::constructor);
            CHECK(cf.constructors().size() == 1);
            CHECK(cf.constructors().find("triangle")->second == Triangle::constructor);

            const ConstructionFactory<Term*> const_cf(cf);
            CHECK(const_cf.constructors().size() == 1);
            CHECK(const_cf.constructors().find("triangle")->second == Triangle::constructor);
        }

        SECTION("Constructor factory clones") {
            ConstructionFactory<Term*> cf("terms");
            cf.registerConstructor("triangle", Triangle::constructor);
            std::unique_ptr<ConstructionFactory<Term*>> clone(cf.clone());
            CHECK(cf.name() == "terms");
            CHECK(clone->name() == "terms");

            CHECK(cf.available() == std::vector<std::string>{"triangle"});
            CHECK(clone->available() == std::vector<std::string>{"triangle"});

            std::unique_ptr<Term> triangle(cf.constructObject("triangle"));
            std::unique_ptr<Term> triangleClone(clone->constructObject("triangle"));
            CHECK(triangle->toString() == triangleClone->toString());
        }
    }

    struct ActivationFactoryAssert : ConstructionFactoryAssert<Activation> {
        explicit ActivationFactoryAssert(ActivationFactory* actual) : ConstructionFactoryAssert(actual) {}
    };

    struct DefuzzifierFactoryAssert : ConstructionFactoryAssert<Defuzzifier> {
        explicit DefuzzifierFactoryAssert(DefuzzifierFactory* actual) : ConstructionFactoryAssert(actual) {}

        DefuzzifierFactoryAssert& construct_weighted(
            const std::string& name, WeightedDefuzzifier::Type type, const WeightedDefuzzifier& expected
        ) {
            CAPTURE(name, type, FllExporter().toString(&expected));
            const DefuzzifierFactory* actualFactory = dynamic_cast<const DefuzzifierFactory*>(actual.get());
            std::unique_ptr<Defuzzifier> obtained(actualFactory->constructWeighted(name, type));
            CHECK(FllExporter().toString(obtained.get()) == FllExporter().toString(&expected));
            return *this;
        }

        DefuzzifierFactoryAssert&
        construct_integral(const std::string& name, int resolution, const IntegralDefuzzifier& expected) {
            CAPTURE(name, resolution, FllExporter().toString(&expected));
            const DefuzzifierFactory* actualFactory = dynamic_cast<const DefuzzifierFactory*>(actual.get());
            std::unique_ptr<Defuzzifier> obtained(actualFactory->constructIntegral(name, resolution));
            CHECK(FllExporter().toString(obtained.get()) == FllExporter().toString(&expected));
            return *this;
        }
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
        SECTION("Clones") {
            ActivationFactoryAssert(new ActivationFactory).clones();
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
        SECTION("Construct integral with parameters") {
            DefuzzifierFactoryAssert(new DefuzzifierFactory)
                .construct_integral("Bisector", 300, Bisector(300))
                .construct_integral("Centroid", 600, Centroid(600))
                .construct_integral("LargestOfMaximum", 900, LargestOfMaximum(900))
                .construct_integral("MeanOfMaximum", 1000, MeanOfMaximum())
                .construct_integral("SmallestOfMaximum", 100, SmallestOfMaximum(100));
        }
        SECTION("Construct integral with parameters") {
            DefuzzifierFactoryAssert(new DefuzzifierFactory)
                .construct_weighted("WeightedAverage", WeightedDefuzzifier::Automatic, WeightedAverage("Automatic"))
                .construct_weighted(
                    "WeightedAverage", WeightedDefuzzifier::TakagiSugeno, WeightedAverage("TakagiSugeno")
                )
                .construct_weighted("WeightedAverage", WeightedDefuzzifier::Tsukamoto, WeightedAverage("Tsukamoto"))
                .construct_weighted(
                    "WeightedSum", WeightedDefuzzifier::Automatic, WeightedSum(WeightedDefuzzifier::Automatic)
                )
                .construct_weighted(
                    "WeightedSum", WeightedDefuzzifier::TakagiSugeno, WeightedSum(WeightedDefuzzifier::TakagiSugeno)
                )
                .construct_weighted(
                    "WeightedSum", WeightedDefuzzifier::Tsukamoto, WeightedSum(WeightedDefuzzifier::Tsukamoto)
                );
        }
        SECTION("Deregister all") {
            DefuzzifierFactoryAssert(new DefuzzifierFactory).deregister_all();
        }
        SECTION("Clones") {
            DefuzzifierFactoryAssert(new DefuzzifierFactory).clones();
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
        SECTION("Clones") {
            HedgeFactoryAssert(new HedgeFactory).clones();
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
        SECTION("Clones") {
            SNormFactoryAssert(new SNormFactory).clones();
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
        SECTION("Clones") {
            TNormFactoryAssert(new TNormFactory).clones();
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
        SECTION("Clones") {
            TermFactoryAssert(new TermFactory).clones();
        }
    }

    template <typename T>
    struct CloningFactoryAssert {
        std::unique_ptr<CloningFactory<T*>> actual;

        using Clone = std::tuple<const std::string&, const T&>;

        explicit CloningFactoryAssert(CloningFactory<T*>* actual) : actual(actual) {}

        CloningFactoryAssert& has_class_name(const std::string& name) {
            CHECK(actual->name() == name);
            return *this;
        }

        CloningFactoryAssert& contains(const std::vector<std::string>& names, bool contains = true) {
            CAPTURE(names, contains);
            for (const std::string& name : names)
                CHECK(actual->hasObject(name) == contains);
            return *this;
        }

        CloningFactoryAssert& copies_exactly(const std::vector<Clone>& clones) {
            std::vector<std::string> expected;
            for (const auto& name_clone : clones) {
                std::string name;
                Clone clone;
                std::tie(name, clone) = name_clone;
                const std::string& fll = fl::FllExporter().toString(&clone);
                CAPTURE(name, fll);
                expected.push_back(name);
                CHECK(actual->hasObject(name));
                CHECK(fl::FllExporter().toString(actual->getObject(name)) == fll);
                std::unique_ptr<T> object(actual->cloneObject(name));
                CHECK_THAT(fl::FllExporter().toString(object.get()), Catch::Matchers::Equals(fll));
            }
            CHECK_THAT(actual->available(), Catch::Matchers::UnorderedEquals(expected));
            return *this;
        }

        CloningFactoryAssert& deregister_all() {
            for (const auto& clone : actual->available()) {
                CAPTURE(clone);
                CHECK(actual->hasObject(clone));
                actual->deregisterObject(clone);
                CHECK(not actual->hasObject(clone));
            }
            CHECK(actual->objects().empty());
            return *this;
        }
    };

    struct FunctionFactoryAssert : CloningFactoryAssert<Function::Element> {
        explicit FunctionFactoryAssert(FunctionFactory* actual) : CloningFactoryAssert(actual) {}

        const scalar pi = std::atan(1) * 4;

        const std::vector<scalar> values{
            fl::nan,
            0,
            1,
            2,
            4,
            5,
            10,
            100,
            1000,
            10000,
            pi / 4,
            pi / 2,
            3 * pi / 4,
            pi,
            5 * pi / 4,
            3 * pi / 2,
            7 * pi / 4,
            2 * pi,
            fl::inf
        };

        FunctionFactoryAssert& precedence_is_the_same(const std::string& a, const std::string& b) {
            CAPTURE(actual->getObject(a)->toString());
            CAPTURE(actual->getObject(b)->toString());
            CHECK(actual->getObject(a)->precedence == actual->getObject(b)->precedence);
            return *this;
        }

        FunctionFactoryAssert& precedence_is_higher(const std::string& a, const std::string& b) {
            CAPTURE(actual->getObject(a)->toString());
            CAPTURE(actual->getObject(b)->toString());
            CHECK(actual->getObject(a)->precedence > actual->getObject(b)->precedence);
            return *this;
        }

        FunctionFactoryAssert& operation_is(const std::string& name, scalar parameter, scalar expected) {
            CAPTURE(name, parameter, expected);
            CAPTURE(actual->getObject(name)->toString());
            CHECK_THAT(actual->getObject(name)->unary(parameter), Approximates(expected));
            return *this;
        }

        FunctionFactoryAssert& operation_is(const std::string& name, scalar a, scalar b, scalar expected) {
            CAPTURE(name, a, b, expected);
            CAPTURE(actual->getObject(name)->toString());
            CHECK_THAT(actual->getObject(name)->binary(a, b), Approximates(expected));
            return *this;
        }

        FunctionFactoryAssert& unary_operation_equals(const std::string& name, Function::Unary expected) {
            CHECK(not values.empty());
            for (auto x : values) {
                CAPTURE(name, x, expected(x));
                auto obtained = actual->getObject(name)->unary;
                CHECK_THAT(obtained(x), Approximates(expected(x)));
                CHECK_THAT(obtained(-x), Approximates(expected(-x)));
            }
            return *this;
        }

        FunctionFactoryAssert& binary_operation_equals(const std::string& name, Function::Binary expected) {
            CHECK(not values.empty());
            for (auto x : values) {
                for (auto y : values) {
                    CAPTURE(name, x, y, expected(x, y));
                    auto obtained = actual->getObject(name)->binary;
                    CHECK_THAT(obtained(x, y), Approximates(expected(x, y)));
                    CHECK_THAT(obtained(-x, -y), Approximates(expected(-x, -y)));
                    CHECK_THAT(obtained(x, -y), Approximates(expected(x, -y)));
                    CHECK_THAT(obtained(-x, y), Approximates(expected(-x, y)));
                }
            }
            return *this;
        }
    };

    const std::vector<std::string> operators = {"!", "~", "%", "^", "*", "/", "+", "-", "and", "or"};
    const std::vector<std::string> functions
        = {"abs",   "acos",  "asin",  "atan",  "atan2", "ceil", "cos",  "cosh",  "eq",   "exp",
           "fabs",  "floor", "fmod",  "ge",    "gt",    "le",   "log",  "log10", "lt",   "max",
           "min",   "neq",   "pow",   "round", "sin",   "sinh", "sqrt", "tan",   "tanh",
#if defined(FL_UNIX) && !defined(FL_USE_FLOAT)  // found in Unix when using double precision. not found in Windows.
           "acosh", "asinh", "atanh", "log1p"
#endif
    };
    TEST_CASE("FunctionFactory", "[factory][function]") {
        SECTION("Cloning empty object returns null") {
            FunctionFactory ff;
            ff.clear();
            ff.registerObject("null", fl::null);
            CHECK(ff.cloneObject("null") == fl::null);
            ff.deregisterObject("null");
            CHECK(ff.available().empty());
        }

        SECTION("Operators available") {
            CHECK_THAT(FunctionFactory().availableOperators(), Catch::Matchers::UnorderedEquals(operators));
        }
        SECTION("Functions available") {
            CHECK_THAT(FunctionFactory().availableFunctions(), Catch::Matchers::UnorderedEquals(functions));
        }

        SECTION("Available") {
            std::vector<std::string> expected;
            std::copy(operators.begin(), operators.end(), std::back_inserter(expected));
            std::copy(functions.begin(), functions.end(), std::back_inserter(expected));
            CHECK_THAT(FunctionFactory().available(), Catch::Matchers::UnorderedEquals(expected));
        }

        SECTION("Precedence is the same") {
            FunctionFactoryAssert(new FunctionFactory)
                .precedence_is_the_same("!", "~")
                .precedence_is_the_same("*", "/")
                .precedence_is_the_same("/", "%")
                .precedence_is_the_same("+", "-");
        }
        SECTION("Precedence is higher") {
            FunctionFactoryAssert(new FunctionFactory)
                .precedence_is_higher("!", "^")
                .precedence_is_higher("^", "%")
                .precedence_is_higher("*", "-")
                .precedence_is_higher("+", "and")
                .precedence_is_higher("and", "or");
        }

        SECTION("Precedence is correct") {
            Function f("f", "(10 + 5) * 2 - 3 / 4 ^ 2");
            f.load();
            CHECK_THAT(f.evaluate(), Approximates(29.8125));
        }

        SECTION("Unary Operators") {
            FunctionFactoryAssert(new FunctionFactory)
                .unary_operation_equals("!", &Op::logicalNot)
                .operation_is("!", 0, 1)
                .operation_is("!", 1, 0)
                .unary_operation_equals("~", &Op::negate)
                .operation_is("~", 1, -1)
                .operation_is("~", -2, 2)
                .operation_is("~", 0, 0);
        }
        SECTION("Binary Operators") {
            FunctionFactoryAssert(new FunctionFactory)
                .binary_operation_equals("^", &std::pow)
                .operation_is("^", 3, 3, 27)
                .operation_is("^", 9, 0.5, 3)
                .binary_operation_equals("*", &Op::multiply)
                .operation_is("*", -2, 3, -6)
                .operation_is("*", 3, -2, -6)
                .operation_is("*", 0, 0, 0)
                .binary_operation_equals("/", &Op::divide)
                .operation_is("/", 6, 3, 2)
                .operation_is("/", 3, 6, 0.5)
                .operation_is("/", 0, 0, fl::nan)
                .operation_is("/", 1, 0, fl::inf)
                .operation_is("/", -1, 0, -fl::inf)
                .binary_operation_equals("%", &Op::modulo)
                .operation_is("%", 6, 3, 0)
                .operation_is("%", 3, 6, 3)
                .operation_is("%", 3.5, 6, 3.5)
                .operation_is("%", 6, 3.5, 2.5)
                .binary_operation_equals("+", &Op::add)
                .operation_is("+", 2, 3, 5)
                .operation_is("+", 2, -3, -1)
                .binary_operation_equals("-", &Op::subtract)
                .operation_is("-", 2, 3, -1)
                .operation_is("-", 2, -3, 5)
                .binary_operation_equals("and", &Op::logicalAnd)
                .operation_is("and", 1, 0, 0)
                .operation_is("and", 1, 1, 1)
                .binary_operation_equals("or", &Op::logicalOr)
                .operation_is("or", 1, 0, 1)
                .operation_is("or", 0, 0, 0);
        }
        SECTION("Unary functions") {
            FunctionFactoryAssert(new FunctionFactory)
                .unary_operation_equals("abs", &std::abs)
                .unary_operation_equals("acos", &std::acos)
                .unary_operation_equals("asin", &std::asin)
                .unary_operation_equals("atan", &std::atan)
                .unary_operation_equals("ceil", &std::ceil)
                .unary_operation_equals("cos", &std::cos)
                .unary_operation_equals("cosh", &std::cosh)
                .unary_operation_equals("exp", &std::exp)
                .unary_operation_equals("fabs", &std::fabs)
                .unary_operation_equals("floor", &std::floor)
                .unary_operation_equals("log10", &std::log10)
                .unary_operation_equals("log", &std::log)
                .unary_operation_equals("round", &Op::round)
                .unary_operation_equals("sin", &std::sin)
                .unary_operation_equals("sinh", &std::sinh)
                .unary_operation_equals("sqrt", &std::sqrt)
                .unary_operation_equals("tan", &std::tan)
                .unary_operation_equals("tanh", &std::tanh);

#if defined(FL_UNIX) && !defined(FL_USE_FLOAT)
            FunctionFactoryAssert(new FunctionFactory)
                .unary_operation_equals("log1p", &log1p)
                .unary_operation_equals("acosh", &acosh)
                .unary_operation_equals("asinh", &asinh)
                .unary_operation_equals("atanh", &atanh);
#endif
        }

        SECTION("Binary Functions") {
            FunctionFactoryAssert(new FunctionFactory)
                .binary_operation_equals("atan2", &std::atan2)
                .binary_operation_equals("eq", &Op::eq)
                .binary_operation_equals("fmod", &std::fmod)
                .binary_operation_equals("ge", &Op::ge)
                .binary_operation_equals("gt", &Op::gt)
                .binary_operation_equals("le", &Op::le)
                .binary_operation_equals("lt", &Op::lt)
                .binary_operation_equals("max", &Op::max)
                .binary_operation_equals("min", &Op::min)
                .binary_operation_equals("neq", &Op::neq)
                .binary_operation_equals("pow", &std::pow);
        }

        SECTION("Cloning unregistered object raises exception") {
            const std::string expected = "[cloning error] Function object by name <X> not registered";
            FunctionFactory ff;
            CHECK_THROWS_AS(ff.cloneObject("X"), fl::Exception);
            CHECK_THROWS_WITH(ff.cloneObject("X"), Catch::Matchers::StartsWith(expected));
        }

        SECTION("Deregister all") {
            FunctionFactoryAssert(new FunctionFactory).deregister_all();
        }
        SECTION("Assign constructor") {
            FunctionFactory only_operators;
            for (auto function : only_operators.availableFunctions())
                only_operators.deregisterObject(function);
            FunctionFactory ff;
            ff = only_operators;
            CHECK(ff.availableFunctions() == std::vector<std::string>{});
            CHECK_THAT(ff.availableOperators(), Catch::Matchers::UnorderedEquals(operators));
        }

        SECTION("Copy constructor with operators") {
            FunctionFactory only_operators;
            for (auto function : only_operators.availableFunctions())
                only_operators.deregisterObject(function);
            FunctionFactory ff(only_operators);
            CHECK(ff.availableFunctions() == std::vector<std::string>{});
            CHECK_THAT(ff.availableOperators(), Catch::Matchers::UnorderedEquals(operators));
        }
        SECTION("Copy constructor with functions") {
            FunctionFactory only_functions;
            for (auto operator_ : only_functions.availableOperators())
                only_functions.deregisterObject(operator_);
            FunctionFactory ff(only_functions);
            CHECK(ff.availableOperators() == std::vector<std::string>{});
            CHECK_THAT(ff.availableFunctions(), Catch::Matchers::UnorderedEquals(functions));
        }
    }

    TEST_CASE("Factory Manager", "[factory]") {
        SECTION("Default factories") {
            FactoryManager fm;
            CHECK(fm.tnorm()->name() == "TNorm");
            CHECK(fm.snorm()->name() == "SNorm");
            CHECK(fm.activation()->name() == "Activation");
            CHECK(fm.defuzzifier()->name() == "Defuzzifier");
            CHECK(fm.term()->name() == "Term");
            CHECK(fm.hedge()->name() == "Hedge");
            CHECK(fm.function()->name() == "Function");
        }

        struct CustomTNormFactory : TNormFactory {
            CustomTNormFactory() : TNormFactory("CustomTNorm") {}
        };

        struct CustomSNormFactory : SNormFactory {
            CustomSNormFactory() : SNormFactory("CustomSNorm") {}
        };

        struct CustomDefuzziferFactory : DefuzzifierFactory {
            CustomDefuzziferFactory() : DefuzzifierFactory("CustomDefuzzifier") {}
        };

        struct CustomHedgeFactory : HedgeFactory {
            CustomHedgeFactory() : HedgeFactory("CustomHedge") {}
        };

        struct CustomActivationFactory : ActivationFactory {
            CustomActivationFactory() : ActivationFactory("CustomActivation") {}
        };

        struct CustomTermFactory : TermFactory {
            CustomTermFactory() : TermFactory("CustomTerm") {}
        };

        struct CustomFunctionFactory : FunctionFactory {
            CustomFunctionFactory() : FunctionFactory("CustomFunction") {}
        };

        SECTION("Constructor of Custom factories") {
            FactoryManager fm(
                new CustomTNormFactory,
                new CustomSNormFactory,
                new CustomActivationFactory,
                new CustomDefuzziferFactory,
                new CustomTermFactory,
                new CustomHedgeFactory,
                new CustomFunctionFactory
            );
            CHECK(fm.tnorm()->name() == "CustomTNorm");
            CHECK(fm.snorm()->name() == "CustomSNorm");
            CHECK(fm.activation()->name() == "CustomActivation");
            CHECK(fm.defuzzifier()->name() == "CustomDefuzzifier");
            CHECK(fm.term()->name() == "CustomTerm");
            CHECK(fm.hedge()->name() == "CustomHedge");
            CHECK(fm.function()->name() == "CustomFunction");
        }

        SECTION("Setter of Custom factories") {
            FactoryManager fm;

            fm.setTnorm(new CustomTNormFactory);
            fm.setSnorm(new CustomSNormFactory);
            fm.setActivation(new CustomActivationFactory);
            fm.setDefuzzifier(new CustomDefuzziferFactory);
            fm.setTerm(new CustomTermFactory);
            fm.setHedge(new CustomHedgeFactory);
            fm.setFunction(new CustomFunctionFactory);

            CHECK(fm.tnorm()->name() == "CustomTNorm");
            CHECK(fm.snorm()->name() == "CustomSNorm");
            CHECK(fm.activation()->name() == "CustomActivation");
            CHECK(fm.defuzzifier()->name() == "CustomDefuzzifier");
            CHECK(fm.term()->name() == "CustomTerm");
            CHECK(fm.hedge()->name() == "CustomHedge");
            CHECK(fm.function()->name() == "CustomFunction");
        }

        SECTION("Copy constructor of custom factories") {
            FactoryManager customFactoryManager(
                new CustomTNormFactory,
                new CustomSNormFactory,
                new CustomActivationFactory,
                new CustomDefuzziferFactory,
                new CustomTermFactory,
                new CustomHedgeFactory,
                new CustomFunctionFactory
            );
            FactoryManager fm(customFactoryManager);

            CHECK(fm.tnorm()->name() == "CustomTNorm");
            CHECK(fm.snorm()->name() == "CustomSNorm");
            CHECK(fm.activation()->name() == "CustomActivation");
            CHECK(fm.defuzzifier()->name() == "CustomDefuzzifier");
            CHECK(fm.term()->name() == "CustomTerm");
            CHECK(fm.hedge()->name() == "CustomHedge");
            CHECK(fm.function()->name() == "CustomFunction");
        }

        SECTION("Assign of custom factories") {
            FactoryManager customFactoryManager(
                new CustomTNormFactory,
                new CustomSNormFactory,
                new CustomActivationFactory,
                new CustomDefuzziferFactory,
                new CustomTermFactory,
                new CustomHedgeFactory,
                new CustomFunctionFactory
            );
            FactoryManager fm;
            fm = customFactoryManager;

            CHECK(fm.tnorm()->name() == "CustomTNorm");
            CHECK(fm.snorm()->name() == "CustomSNorm");
            CHECK(fm.activation()->name() == "CustomActivation");
            CHECK(fm.defuzzifier()->name() == "CustomDefuzzifier");
            CHECK(fm.term()->name() == "CustomTerm");
            CHECK(fm.hedge()->name() == "CustomHedge");
            CHECK(fm.function()->name() == "CustomFunction");
        }
    }
}}
