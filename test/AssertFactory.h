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

#ifndef FL_TEST_ASSERT_FACTORY_H
#define FL_TEST_ASSERT_FACTORY_H

#include "test/Headers.h"

namespace fuzzylite { namespace test {
    template <typename T>
    struct AssertConstructionFactory {
        using Constructor = std::tuple<std::string, typename ConstructionFactory<T*>::Constructor, std::string>;
        std::unique_ptr<ConstructionFactory<T*>> actual;

        explicit AssertConstructionFactory(ConstructionFactory<T*>* actual) : actual(actual) {}

        AssertConstructionFactory& has_class_name(const std::string& name) {
            CHECK(actual->name() == name);
            return *this;
        }

        AssertConstructionFactory& contains(const std::vector<std::string>& names, bool contains = true) {
            CAPTURE(names, contains);
            for (const std::string& name : names)
                CHECK(actual->hasConstructor(name) == contains);
            return *this;
        }

        AssertConstructionFactory&
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

        AssertConstructionFactory& constructs_exactly(const std::vector<Constructor>& constructors) {
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

        AssertConstructionFactory& deregister_all() {
            for (const auto& constructor : actual->available()) {
                CAPTURE(constructor);
                CHECK(actual->hasConstructor(constructor));
                actual->deregisterConstructor(constructor);
                CHECK(not actual->hasConstructor(constructor));
            }
            CHECK(actual->constructors().empty());
            return *this;
        }

        AssertConstructionFactory& clones() {
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

    struct AssertActivationFactory : AssertConstructionFactory<Activation> {
        explicit AssertActivationFactory(ActivationFactory* actual) : AssertConstructionFactory(actual) {}
    };

    struct AssertDefuzzifierFactory : AssertConstructionFactory<Defuzzifier> {
        explicit AssertDefuzzifierFactory(DefuzzifierFactory* actual) : AssertConstructionFactory(actual) {}

        AssertDefuzzifierFactory& construct_weighted(
            const std::string& name, WeightedDefuzzifier::Type type, const WeightedDefuzzifier& expected
        ) {
            CAPTURE(name, type, FllExporter().toString(&expected));
            const DefuzzifierFactory* actualFactory = dynamic_cast<const DefuzzifierFactory*>(actual.get());
            std::unique_ptr<Defuzzifier> obtained(actualFactory->constructWeighted(name, type));
            CHECK(FllExporter().toString(obtained.get()) == FllExporter().toString(&expected));
            return *this;
        }

        AssertDefuzzifierFactory&
        construct_integral(const std::string& name, int resolution, const IntegralDefuzzifier& expected) {
            CAPTURE(name, resolution, FllExporter().toString(&expected));
            const DefuzzifierFactory* actualFactory = dynamic_cast<const DefuzzifierFactory*>(actual.get());
            std::unique_ptr<Defuzzifier> obtained(actualFactory->constructIntegral(name, resolution));
            CHECK(FllExporter().toString(obtained.get()) == FllExporter().toString(&expected));
            return *this;
        }
    };

    struct AssertHedgeFactory : AssertConstructionFactory<Hedge> {
        explicit AssertHedgeFactory(HedgeFactory* actual) : AssertConstructionFactory(actual) {}
    };

    struct AssertSNormFactory : AssertConstructionFactory<SNorm> {
        explicit AssertSNormFactory(SNormFactory* actual) : AssertConstructionFactory(actual) {}
    };

    struct AssertTNormFactory : AssertConstructionFactory<TNorm> {
        explicit AssertTNormFactory(TNormFactory* actual) : AssertConstructionFactory(actual) {}
    };

    struct AssertTermFactory : AssertConstructionFactory<Term> {
        explicit AssertTermFactory(TermFactory* actual) : AssertConstructionFactory(actual) {}
    };

    template <typename T>
    struct AssertCloningFactory {
        std::unique_ptr<CloningFactory<T*>> actual;

        using Clone = std::tuple<const std::string&, const T&>;

        explicit AssertCloningFactory(CloningFactory<T*>* actual) : actual(actual) {}

        AssertCloningFactory& has_class_name(const std::string& name) {
            CHECK(actual->name() == name);
            return *this;
        }

        AssertCloningFactory& contains(const std::vector<std::string>& names, bool contains = true) {
            CAPTURE(names, contains);
            for (const std::string& name : names)
                CHECK(actual->hasObject(name) == contains);
            return *this;
        }

        AssertCloningFactory& copies_exactly(const std::vector<Clone>& clones) {
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

        AssertCloningFactory& deregister_all() {
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

    struct AssertFunctionFactory : AssertCloningFactory<Function::Element> {
        explicit AssertFunctionFactory(FunctionFactory* actual) : AssertCloningFactory(actual) {}

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

        AssertFunctionFactory& precedence_is_the_same(const std::string& a, const std::string& b) {
            CAPTURE(actual->getObject(a)->toString());
            CAPTURE(actual->getObject(b)->toString());
            CHECK(actual->getObject(a)->precedence == actual->getObject(b)->precedence);
            return *this;
        }

        AssertFunctionFactory& precedence_is_higher(const std::string& a, const std::string& b) {
            CAPTURE(actual->getObject(a)->toString());
            CAPTURE(actual->getObject(b)->toString());
            CHECK(actual->getObject(a)->precedence > actual->getObject(b)->precedence);
            return *this;
        }

        AssertFunctionFactory& operation_is(const std::string& name, scalar parameter, scalar expected) {
            CAPTURE(name, parameter, expected);
            CAPTURE(actual->getObject(name)->toString());
            CHECK_THAT(actual->getObject(name)->unary(parameter), Approximates(expected));
            return *this;
        }

        AssertFunctionFactory& operation_is(const std::string& name, scalar a, scalar b, scalar expected) {
            CAPTURE(name, a, b, expected);
            CAPTURE(actual->getObject(name)->toString());
            CHECK_THAT(actual->getObject(name)->binary(a, b), Approximates(expected));
            return *this;
        }

        AssertFunctionFactory& unary_operation_equals(const std::string& name, Function::Unary expected) {
            CHECK(not values.empty());
            for (auto x : values) {
                CAPTURE(name, x, expected(x));
                auto obtained = actual->getObject(name)->unary;
                CHECK_THAT(obtained(x), Approximates(expected(x)));
                CHECK_THAT(obtained(-x), Approximates(expected(-x)));
            }
            return *this;
        }

        AssertFunctionFactory& binary_operation_equals(const std::string& name, Function::Binary expected) {
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

}}
#endif
