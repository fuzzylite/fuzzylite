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

    struct AssertConstructor {
        template <typename T>
        AssertConstructor& is_complete() {
            has_default<T>();
            supports_copy<T>();
            supports_move<T>();
            // TODO: is_noexcept<T>(); in fuzzylite 8
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_copy_and_move(const T test, CompareMethod method) {
            // is_complete<T>();
            can_copy(test, method);
            can_move(test, method);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_copy(const T test, CompareMethod method) {
            can_copy_construct(test, method);
            can_copy_assign(test, method);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_move(const T test, CompareMethod method) {
            can_move_construct(test, method);
            can_move_assign(test, method);
            return *this;
        }

        template <typename T>
        AssertConstructor& has_default() {
            CHECK(std::is_constructible<T>::value);
            T constructor;
            (void)constructor;
            return *this;
        }

        template <typename T>
        AssertConstructor& is_noexcept() {
            CHECK(std::is_nothrow_constructible<T>());
            return *this;
        }

        template <typename T>
        AssertConstructor& supports_copy() {
            has_copy_constructor<T>();
            has_copy_assignment<T>();
            return *this;
        }

        template <typename T>
        AssertConstructor& supports_move() {
            has_move_constructor<T>();
            has_move_assignment<T>();
            return *this;
        }

        template <typename T>
        AssertConstructor& has_copy_constructor() {
            CHECK(std::is_copy_constructible<T>::value);
            T original;
            T copy(original);
            (void)copy;
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_copy_construct(const T& test, CompareMethod method) {
            has_copy_constructor<T>();
            const T copy(test);
            CHECK((test.*method)() == (copy.*method)());
            return *this;
        }

        template <typename T>
        AssertConstructor& has_copy_assignment() {
            CHECK(std::is_copy_assignable<T>::value);
            T original;
            T copyAssigned;
            copyAssigned = original;
            CHECK(std::is_same<decltype(copyAssigned = original), T&>::value);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_copy_assign(const T& test, CompareMethod method) {
            has_copy_constructor<T>();
            const T copyAssigned = test;
            CHECK((test.*method)() == (copyAssigned.*method)());
            return *this;
        }

        template <typename T>
        AssertConstructor& has_move_constructor() {
            CHECK(std::is_move_constructible<T>::value);
            T original;
            T moved(std::move(original));
            (void)moved;
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_move_construct(const T& test, CompareMethod method) {
            has_move_constructor<T>();
            auto expected = (test.*method)();
            T moved(std::move(test));
            auto obtained = (moved.*method)();
            CHECK(obtained == expected);
            return *this;
        }

        template <typename T>
        AssertConstructor& has_move_assignment() {
            CHECK(std::is_move_assignable<T>::value);
            T original;
            T moveAssigned;
            moveAssigned = std::move(original);
            CHECK(std::is_same<decltype(moveAssigned = std::move(original)), T&>::value);
            return *this;
        }

        template <typename T, typename CompareMethod>
        AssertConstructor& can_move_assign(const T& test, CompareMethod method) {
            has_move_assignment<T>();
            auto expected = (test.*method)();
            T movedAssigned;
            movedAssigned = std::move(test);
            auto obtained = (movedAssigned.*method)();
            CHECK(obtained == expected);
            return *this;
        }
    };

}}
#endif
