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

#include "test/AssertFactory.h"
#include "test/Headers.h"

namespace fuzzylite { namespace test {

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

    TEST_CASE("ActivationFactory", "[factory][activation]") {
        SECTION("Default name") {
            AssertActivationFactory(new ActivationFactory).has_class_name("Activation");
        }
        SECTION("Contains") {
            AssertActivationFactory(new ActivationFactory).contains({"First", "Last", "Threshold"});
        }
        SECTION("Does not contain") {
            AssertActivationFactory(new ActivationFactory).contains({"Second", "Third"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<AssertConstructionFactory<Activation>::Constructor> constructs{
                {"", fl::null, ""},
                {"First", First::constructor, "First 1 0.000"},
                {"General", General::constructor, "General"},
                {"Highest", Highest::constructor, "Highest 1"},
                {"Last", Last::constructor, "Last 1 0.000"},
                {"Lowest", Lowest::constructor, "Lowest 1"},
                {"Proportional", Proportional::constructor, "Proportional"},
                {"Threshold", Threshold::constructor, "Threshold >= 0.000"},
            };
            AssertActivationFactory(new ActivationFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            AssertActivationFactory(new ActivationFactory).deregister_all();
        }
        SECTION("Clones") {
            AssertActivationFactory(new ActivationFactory).clones();
        }
    }

    TEST_CASE("DefuzzifierFactory", "[factory][defuzzifier]") {
        SECTION("Default name") {
            AssertDefuzzifierFactory(new DefuzzifierFactory).has_class_name("Defuzzifier");
        }
        SECTION("Contains") {
            AssertDefuzzifierFactory(new DefuzzifierFactory).contains({"Bisector", "Centroid", "WeightedAverage"});
        }
        SECTION("Does not contain") {
            AssertDefuzzifierFactory(new DefuzzifierFactory).contains({"CenterOfGravity", "CoG"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<AssertConstructionFactory<Defuzzifier>::Constructor> constructs{
                {"", fl::null, ""},
                {"Bisector", Bisector::constructor, "Bisector"},
                {"Centroid", Centroid::constructor, "Centroid"},
                {"LargestOfMaximum", LargestOfMaximum::constructor, "LargestOfMaximum"},
                {"MeanOfMaximum", MeanOfMaximum::constructor, "MeanOfMaximum"},
                {"SmallestOfMaximum", SmallestOfMaximum::constructor, "SmallestOfMaximum"},
                {"WeightedAverage", WeightedAverage::constructor, "WeightedAverage"},
                {"WeightedSum", WeightedSum::constructor, "WeightedSum"},
            };
            AssertDefuzzifierFactory(new DefuzzifierFactory).constructs_exactly(constructs);
        }
        SECTION("Construct integral with parameters") {
            AssertDefuzzifierFactory(new DefuzzifierFactory)
                .construct_integral("Bisector", 300, Bisector(300))
                .construct_integral("Centroid", 600, Centroid(600))
                .construct_integral("LargestOfMaximum", 900, LargestOfMaximum(900))
                .construct_integral("MeanOfMaximum", 1000, MeanOfMaximum())
                .construct_integral("SmallestOfMaximum", 100, SmallestOfMaximum(100));
        }
        SECTION("Construct integral with parameters") {
            AssertDefuzzifierFactory(new DefuzzifierFactory)
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
            AssertDefuzzifierFactory(new DefuzzifierFactory).deregister_all();
        }
        SECTION("Clones") {
            AssertDefuzzifierFactory(new DefuzzifierFactory).clones();
        }
    }

    TEST_CASE("HedgeFactory", "[factory][hedge]") {
        SECTION("Default name") {
            AssertHedgeFactory(new HedgeFactory).has_class_name("Hedge");
        }
        SECTION("Contains") {
            AssertHedgeFactory(new HedgeFactory).contains({"very", "extremely", "not"});
        }
        SECTION("Does not contain") {
            AssertHedgeFactory(new HedgeFactory).contains({"way", "often"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<AssertConstructionFactory<Hedge>::Constructor> constructs{
                {"", fl::null, ""},
                {"any", Any::constructor, "any"},
                {"extremely", Extremely::constructor, "extremely"},
                {"not", Not::constructor, "not"},
                {"seldom", Seldom::constructor, "seldom"},
                {"somewhat", Somewhat::constructor, "somewhat"},
                {"very", Very::constructor, "very"},
            };
            AssertHedgeFactory(new HedgeFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            AssertHedgeFactory(new HedgeFactory).deregister_all();
        }
        SECTION("Clones") {
            AssertHedgeFactory(new HedgeFactory).clones();
        }
    }

    TEST_CASE("SNormFactory", "[factory][snorm]") {
        SECTION("Default name") {
            AssertSNormFactory(new SNormFactory).has_class_name("SNorm");
        }
        SECTION("Contains") {
            AssertSNormFactory(new SNormFactory).contains({"AlgebraicSum", "EinsteinSum", "Maximum"});
        }
        SECTION("Does not contain") {
            AssertSNormFactory(new SNormFactory).contains({"AlgebraicProduct", "EinsteinProduct"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<AssertConstructionFactory<SNorm>::Constructor> constructs{
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
            AssertSNormFactory(new SNormFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            AssertSNormFactory(new SNormFactory).deregister_all();
        }
        SECTION("Clones") {
            AssertSNormFactory(new SNormFactory).clones();
        }
    }

    TEST_CASE("TNormFactory", "[factory][tnorm]") {
        SECTION("Default name") {
            AssertTNormFactory(new TNormFactory).has_class_name("TNorm");
        }
        SECTION("Contains") {
            AssertTNormFactory(new TNormFactory).contains({"AlgebraicProduct", "EinsteinProduct", "Minimum"});
        }
        SECTION("Does not contain") {
            AssertTNormFactory(new TNormFactory).contains({"AlgebraicSum", "EinsteinSum"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<AssertConstructionFactory<TNorm>::Constructor> constructs{
                {"", fl::null, ""},
                {"AlgebraicProduct", AlgebraicProduct::constructor, "AlgebraicProduct"},
                {"BoundedDifference", BoundedDifference::constructor, "BoundedDifference"},
                {"DrasticProduct", DrasticProduct::constructor, "DrasticProduct"},
                {"EinsteinProduct", EinsteinProduct::constructor, "EinsteinProduct"},
                {"HamacherProduct", HamacherProduct::constructor, "HamacherProduct"},
                {"Minimum", Minimum::constructor, "Minimum"},
                {"NilpotentMinimum", NilpotentMinimum::constructor, "NilpotentMinimum"},
            };
            AssertTNormFactory(new TNormFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            AssertTNormFactory(new TNormFactory).deregister_all();
        }
        SECTION("Clones") {
            AssertTNormFactory(new TNormFactory).clones();
        }
    }

    TEST_CASE("TermFactory", "[factory][term]") {
        SECTION("Default name") {
            AssertTermFactory(new TermFactory).has_class_name("Term");
        }
        SECTION("Contains") {
            AssertTermFactory(new TermFactory).contains({"Constant", "Function", "Triangle"});
        }
        SECTION("Does not contain") {
            AssertTermFactory(new TermFactory).contains({"Arc", "SemiEllipse"}, false);
        }
        SECTION("Construct exactly") {
            std::vector<AssertConstructionFactory<Term>::Constructor> constructs{
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
            AssertTermFactory(new TermFactory).constructs_exactly(constructs);
        }
        SECTION("Deregister all") {
            AssertTermFactory(new TermFactory).deregister_all();
        }
        SECTION("Clones") {
            AssertTermFactory(new TermFactory).clones();
        }
    }

    const std::vector<std::string> operators = {"!", "~", "%", "^", "*", "/", "+", "-", "and", "or"};
    const std::vector<std::string> functions = {
        "abs",   "acos",  "asin",  "atan",  "atan2", "ceil", "cos",  "cosh",  "eq",   "exp",
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
            AssertFunctionFactory(new FunctionFactory)
                .precedence_is_the_same("!", "~")
                .precedence_is_the_same("*", "/")
                .precedence_is_the_same("/", "%")
                .precedence_is_the_same("+", "-");
        }
        SECTION("Precedence is higher") {
            AssertFunctionFactory(new FunctionFactory)
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
            AssertFunctionFactory(new FunctionFactory)
                .unary_operation_equals("!", &Op::logicalNot)
                .operation_is("!", 0, 1)
                .operation_is("!", 1, 0)
                .unary_operation_equals("~", &Op::negate)
                .operation_is("~", 1, -1)
                .operation_is("~", -2, 2)
                .operation_is("~", 0, 0);
        }
        SECTION("Binary Operators") {
            AssertFunctionFactory(new FunctionFactory)
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
            AssertFunctionFactory(new FunctionFactory)
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
            AssertFunctionFactory(new FunctionFactory)
                .unary_operation_equals("log1p", &log1p)
                .unary_operation_equals("acosh", &acosh)
                .unary_operation_equals("asinh", &asinh)
                .unary_operation_equals("atanh", &atanh);
#endif
        }

        SECTION("Binary Functions") {
            AssertFunctionFactory(new FunctionFactory)
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
            AssertFunctionFactory(new FunctionFactory).deregister_all();
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
