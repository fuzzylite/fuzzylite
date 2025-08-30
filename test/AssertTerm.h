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

#ifndef FL_TEST_ASSERT_TERM_H
#define FL_TEST_ASSERT_TERM_H
#include "test/Headers.h"

namespace fuzzylite { namespace test {

    template <typename T>
    struct AssertTerm {
        std::unique_ptr<T> actual;

        AssertTerm() : actual(std::make_unique<T>()) {}

        AssertTerm(T* pointer) : actual(pointer) {}

        template <typename... Args>
        explicit AssertTerm(Args&&... args) : actual(std::make_unique<T>(std::forward<Args>(args)...)) {}

        /**
         *
         * @deprecated T::constructor will be removed in fuzzylite 8
         */
        AssertTerm& can_construct() {
            std::unique_ptr<T> test(dynamic_cast<T*>(T::constructor()));
            test->setName(actual->getName());
            test->configure(actual->parameters());
            CHECK(FllExporter().toString(test.get()) == FllExporter().toString(actual.get()));
            return *this;
        }

        AssertTerm& can_clone() {
            // throw  Exception("");
            std::unique_ptr<T> test(actual->clone());
            std::string expected = FllExporter().toString(actual.get());
            std::string obtained = FllExporter().toString(test.get());
            CAPTURE(expected, obtained);
            CHECK(expected == obtained);
            return *this;
        }

        AssertTerm& exports_fll(const std::string& obtained, bool checkHeight = true) {
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

        AssertTerm& repr_is(const std::string& obtained) {
            // TODO
            FL_IUNUSED(obtained);
            return *this;
        }

        AssertTerm& is_monotonic(bool monotonic = true) {
            CHECK(this->actual->isMonotonic() == monotonic);
            return *this;
        }

        AssertTerm& is_not_monotonic() {
            return this->is_monotonic(false);
        }

        AssertTerm& with_height(scalar height) {
            this->actual->setHeight(height);
            return *this;
        }

        AssertTerm& takes_parameters(int requiredParameters, bool height = true) {
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

        AssertTerm& configured_as(const std::string& parameters, bool checkEmpty = true) {
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

        AssertTerm& has_memberships(
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

        AssertTerm& has_tsukamotos(const std::map<scalar, scalar>& values) {
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

        AssertTerm& apply(const std::function<void(Term*)>& f) {
            f(this->actual.get());
            return *this;
        }
    };

}}
#endif
