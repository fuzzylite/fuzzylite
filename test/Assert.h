/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#ifndef FL_TEST_ASSERT_H
#define FL_TEST_ASSERT_H

#include <catch2/catch_all.hpp>
#include <memory>

namespace fuzzylite { namespace test {
    template <typename T>
    struct AssertConstructorOf {
        AssertConstructorOf& is_complete() {
            has_default();
            supports_copy();
            supports_move();
            // TODO: is_noexcept<T>(); in fuzzylite 8
            return *this;
        }

        template <typename CompareMethod>
        AssertConstructorOf& can_copy_and_move(const T& test, CompareMethod method) {
            // is_complete<T>();
            can_copy(test, method);
            can_move(test, method);
            return *this;
        }

        template <typename CompareMethod>
        AssertConstructorOf& can_copy(const T& test, CompareMethod method) {
            can_copy_construct(test, method);
            can_copy_assign(test, method);
            return *this;
        }

        template <typename CompareMethod>
        AssertConstructorOf& can_move(const T& test, CompareMethod method) {
            can_move_construct(test, method);
            can_move_assign(test, method);
            return *this;
        }

        AssertConstructorOf& has_default(bool has = true) {
            CHECK(std::is_constructible<T>::value == has);
            return *this;
        }

        AssertConstructorOf& is_noexcept(bool is = true) {
            CHECK(std::is_nothrow_constructible<T>::value == is);
            return *this;
        }

        AssertConstructorOf& supports_copy(bool supports = true) {
            has_copy_constructor(supports);
            has_copy_assignment(supports);
            return *this;
        }

        AssertConstructorOf& supports_move(bool supports = true) {
            has_move_constructor(supports);
            has_move_assignment(supports);
            return *this;
        }

        AssertConstructorOf& has_copy_constructor(bool has = true) {
            CHECK(std::is_copy_constructible<T>::value == has);
            return *this;
        }

        template <typename CompareMethod>
        AssertConstructorOf& can_copy_construct(const T& test, CompareMethod method) {
            has_copy_constructor();
            const T copy(test);
            CHECK((test.*method)() == (copy.*method)());
            return *this;
        }

        AssertConstructorOf& has_copy_assignment(bool has = true) {
            CHECK(std::is_copy_assignable<T>::value == has);
            return *this;
        }

        template <typename CompareMethod>
        AssertConstructorOf& can_copy_assign(const T& test, CompareMethod method) {
            has_copy_assignment();
            T copyAssigned;
            copyAssigned = test;
            CHECK((test.*method)() == (copyAssigned.*method)());
            return *this;
        }

        AssertConstructorOf& has_move_constructor(bool has = true) {
            CHECK(std::is_move_constructible<T>::value == has);
            return *this;
        }

        template <typename CompareMethod>
        AssertConstructorOf& can_move_construct(const T& test, CompareMethod method) {
            has_move_constructor();
            auto expected = (test.*method)();
            // Given const T& test, we create a copy to remove const
            T movable(test);
            CHECK((movable.*method)() == expected);
            T moved(std::move(movable));
            auto obtained = (moved.*method)();
            CHECK(obtained == expected);
            return *this;
        }

        AssertConstructorOf& has_move_assignment(bool has = true) {
            CHECK(std::is_move_assignable<T>::value == has);
            return *this;
        }

        template <typename CompareMethod>
        AssertConstructorOf& can_move_assign(const T& test, CompareMethod method) {
            has_move_assignment();
            // Given const T& test, we create a copy to remove const
            auto expected = (test.*method)();
            T movable(test);
            CHECK((movable.*method)() == expected);

            T movedAssigned;
            movedAssigned = std::move(movable);
            auto obtained = (movedAssigned.*method)();
            CHECK(obtained == expected);
            return *this;
        }
    };

    struct AssertConstructor {
        template <typename T>
        AssertConstructor& is_complete() {
            AssertConstructorOf<T>().is_complete();
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_copy_and_move(const T& test, CompareMethod method) {
            AssertConstructorOf<T>().can_copy_and_move(test, method);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_copy(const T& test, CompareMethod method) {
            AssertConstructorOf<T>().can_copy(test, method);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_move(const T& test, CompareMethod method) {
            AssertConstructorOf<T>().can_move(test, method);
            return *this;
        }

        template <typename T>
        AssertConstructor& has_default(bool has = true) {
            AssertConstructorOf<T>().has_default(has);
            return *this;
        }

        template <typename T>
        AssertConstructor& is_noexcept(bool is = true) {
            AssertConstructorOf<T>().is_noexcept(is);
            return *this;
        }

        template <typename T>
        AssertConstructor& supports_copy(bool supports = true) {
            AssertConstructorOf<T>().supports_copy(supports);
            return *this;
        }

        template <typename T>
        AssertConstructor& supports_move(bool supports = true) {
            AssertConstructorOf<T>().supports_move(supports);
            return *this;
        }

        template <typename T>
        AssertConstructor& has_copy_constructor(bool has = true) {
            AssertConstructorOf<T>().has_copy_constructor(has);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_copy_construct(const T& test, CompareMethod method) {
            AssertConstructorOf<T>().can_copy_construct(test, method);
            return *this;
        }

        template <typename T>
        AssertConstructor& has_copy_assignment(bool has = true) {
            AssertConstructorOf<T>().has_copy_assignment(has);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_copy_assign(const T& test, CompareMethod method) {
            AssertConstructorOf<T>().can_copy_assign(test, method);
            return *this;
        }

        template <typename T>
        AssertConstructor& has_move_constructor(bool has = true) {
            AssertConstructorOf<T>().has_move_constructor(has);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_move_construct(const T& test, CompareMethod method) {
            AssertConstructorOf<T>().can_move_construct(test, method);
            return *this;
        }

        template <typename T>
        AssertConstructor& has_move_assignment(bool has = true) {
            AssertConstructorOf<T>().has_move_assignment(has);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_move_assign(const T& test, CompareMethod method) {
            AssertConstructorOf<T>().can_move_assign(test, method);
            return *this;
        }
    };

}}
#endif
