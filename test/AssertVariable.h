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

#ifndef FL_TEST_ASSERT_VARIABLE_H
#define FL_TEST_ASSERT_VARIABLE_H
#include "test/Headers.h"

namespace fuzzylite { namespace test {

    struct MockDefuzzifier : Centroid {
        scalar return_value;

        explicit MockDefuzzifier(scalar return_value = nan) : return_value(return_value) {}

        scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const override {
            (void)term;
            (void)minimum;
            (void)maximum;
            return return_value;
        }
    };

    class State {
        scalar _previous, _current;

      public:
        explicit State(scalar previous = nan, scalar current = nan) : _previous(previous), _current(current) {}

        State& previous(scalar value) {
            _previous = value;
            return *this;
        }

        State& current(scalar value) {
            _current = value;
            return *this;
        }

        scalar previous() const {
            return _previous;
        }

        scalar current() const {
            return _current;
        }
    };

    class Combination {
        std::string _number;
        State _before, _after;
        scalar _defuzzify;

        enum Selector { BEFORE, AFTER } _selector = BEFORE;

      public:
        explicit Combination(std::string number) :
            _number(number),
            _before(State()),
            _after(State()),
            _defuzzify(nan) {}

        std::string number() const {
            return _number;
        }

        Combination& when() {
            _selector = BEFORE;
            return *this;
        }

        scalar defuzzify() const {
            return _defuzzify;
        }

        Combination& defuzzify(scalar value) {
            _defuzzify = value;
            _selector = AFTER;
            return *this;
        }

        State& state() {
            switch (_selector) {
                case BEFORE:
                    return _before;
                case AFTER:
                    return _after;
                default:
                    return _before;
            }
        }

        Combination& previous(scalar value) {
            state().previous(value);
            return *this;
        }

        Combination& current(scalar value) {
            state().current(value);
            return *this;
        }

        Combination& before(State state) {
            _before = state;
            return *this;
        }

        Combination& after(State state) {
            _after = state;
            return *this;
        }

        State before() const {
            return _before;
        }

        State after() const {
            return _after;
        }
    };

    // curiously recurring template pattern (CRTP):
    // https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
    template <typename DerivedAssert, typename T>
    struct Assert {
        std::unique_ptr<T> variable;
        Assert() = default;

        explicit Assert(std::unique_ptr<T> actual) : variable(std::move(actual)) {}

        explicit Assert(T* actual) : variable(actual) {}

        DerivedAssert& self() {
            return static_cast<DerivedAssert&>(*this);
        }

        auto& has_name(const std::string& name) {
            CHECK(variable->getName() == name);
            return self();
        }

        auto& has_description(const std::string& description) {
            CHECK(variable->getDescription() == description);
            return self();
        }

        auto& is_enabled(bool enabled = true) {
            CHECK(variable->isEnabled() == enabled);
            return self();
        }

        auto& has_type(Variable::Type type) {
            CHECK(variable->type() == type);
            return self();
        }

        auto& has_range(scalar minimum, scalar maximum) {
            CHECK_THAT(variable->getMinimum(), Approximates(minimum));
            CHECK_THAT(variable->getMaximum(), Approximates(maximum));
            return self();
        }

        auto& has_value(scalar value) {
            CHECK_THAT(variable->getValue(), Approximates(value));
            return self();
        }

        auto& locks_value_in_range(bool locks = true) {
            CHECK(variable->isLockValueInRange() == locks);
            return self();
        }

        auto& exports_fll(const std::string& fll) {
            CHECK(variable->toString() == fll);
            return self();
        }

        auto& exports_fll(const std::vector<std::string>& expected) {
            auto obtained = Op::split(variable->toString(), "\n");
            for (std::size_t i = 0; i < obtained.size(); ++i)
                obtained[i] = Op::trim(obtained[i]);
            CHECK(obtained == expected);
            return self();
        }

        auto& fuzzify(scalar value, const std::string& expected) {
            auto obtained = variable->fuzzify(value);
            CHECK(obtained == expected);
            return self();
        }

        auto& fuzzy_values(std::vector<std::pair<scalar, std::string>> values_expected) {
            for (const auto& pair : values_expected) {
                const std::string& expected = pair.second;
                auto obtained = variable->fuzzify(pair.first);
                CHECK(obtained == expected);
            }
            return self();
        }

        auto& highest_membership(scalar x, const Activated& expected) {
            scalar degree;
            Term* highest = variable->highestMembership(x, &degree);
            CHECK(highest == expected.getTerm());
            CHECK_THAT(degree, Approximates(expected.getDegree()));
            return self();
        }

        auto& highest_activation(scalar x, std::vector<Activated> expected) {
            CAPTURE(x);
            auto maxActivations = variable->maxActivations(x);
            REQUIRE(maxActivations.size() == expected.size());
            for (std::size_t i = 0; i < expected.size(); ++i) {
                CHECK(maxActivations.at(i).getTerm() == expected.at(i).getTerm());
                CHECK_THAT(maxActivations.at(i).getDegree(), Approximates(expected.at(i).getDegree()));
            }
            if (expected.size() == 1)
                highest_membership(x, expected.front());
            return self();
        }

        auto& highest_activation(const std::vector<std::pair<scalar, std::vector<Activated>>>& highestActivations) {
            for (auto& pair : highestActivations)
                highest_activation(pair.first, pair.second);
            return self();
        }

        virtual DerivedAssert& equals(const T& another) {
            CHECK(variable->getName() == another.getName());
            CHECK(variable->getDescription() == another.getDescription());
            CHECK(variable->isEnabled() == another.isEnabled());
            CHECK(variable->isLockValueInRange() == another.isLockValueInRange());

            CHECK_THAT(variable->getMinimum(), Approximates(another.getMinimum()));
            CHECK_THAT(variable->getMaximum(), Approximates(another.getMaximum()));
            CHECK_THAT(variable->getValue(), Approximates(another.getValue()));

            CHECK(variable->toString() == another.toString());
            return self();
        }

        auto& can_clone() {
            auto clone = std::unique_ptr<T>(variable->clone());
            return equals(*clone.get());
        }
    };

    struct AssertVariable : Assert<AssertVariable, Variable> {
        using Assert::Assert;  // inherit constructors
    };

    struct AssertInputVariable : Assert<AssertInputVariable, InputVariable> {
        using Assert::Assert;  // inherit constructors

        auto& has_fuzzy_value(scalar input, const std::string& expected) {
            const scalar previousInput = variable->getValue();
            variable->setValue(input);
            CHECK(variable->fuzzyInputValue() == expected);
            variable->setValue(previousInput);
            return self();
        }

        auto& has_fuzzy_values(const std::vector<std::pair<scalar, std::string>> expected) {
            for (auto& pair : expected)
                has_fuzzy_value(pair.first, pair.second);
            return self();
        }

        AssertInputVariable& equals(const InputVariable& another) override {
            return Assert::equals(another);
        }
    };

    struct AssertOutputVariable : Assert<AssertOutputVariable, OutputVariable> {
        using Assert::Assert;

        auto& has_previous_value(scalar value) {
            CHECK_THAT(variable->getPreviousValue(), Approximates(value));
            return self();
        }

        auto& has_default_value(scalar value) {
            CHECK_THAT(variable->getDefaultValue(), Approximates(value));
            return self();
        }

        auto& locks_previous_value(bool locks = true) {
            CHECK(variable->isLockPreviousValue() == locks);
            return self();
        }

        auto& has_defuzzifier(const std::string& fll) {
            CHECK(FllExporter().toString(variable->getDefuzzifier()) == fll);
            return self();
        }

        auto& has_aggregation(const std::string& fll) {
            CHECK(FllExporter().toString(variable->getAggregation()) == fll);
            return self();
        }

        auto& when_fuzzy_output_is(const std::vector<Activated>& terms) {
            variable->fuzzyOutput()->clear();
            variable->fuzzyOutput()->setTerms(terms);
            return self();
        }

        auto& when_defuzzify() {
            variable->defuzzify();
            return self();
        }

        auto& when_mock_defuzzify(scalar return_value) {
            variable->setDefuzzifier(new MockDefuzzifier(return_value));
            variable->defuzzify();
            return self();
        }

        auto& has_fuzzy_value(const std::string& value) {
            CHECK(variable->fuzzyOutputValue() == value);
            return self();
        }

        AssertOutputVariable& equals(const OutputVariable& another) override {
            Assert::equals(another);
            CHECK_THAT(variable->getDefaultValue(), Approximates(another.getDefaultValue()));
            CHECK_THAT(variable->getPreviousValue(), Approximates(another.getPreviousValue()));
            CHECK(
                FllExporter().toString(variable->getDefuzzifier()) == FllExporter().toString(another.getDefuzzifier())
            );
            CHECK(variable->isLockPreviousValue() == another.isLockPreviousValue());
            return self();
        }

        AssertOutputVariable& fsm(const std::vector<Combination>& combinations) {
            for (auto& combination : combinations) {
                SECTION("Combination " + combination.number()) {
                    CAPTURE(combination.number());
                    variable->setPreviousValue(combination.before().previous());
                    variable->setValue(combination.before().current());
                    variable->setDefuzzifier(new MockDefuzzifier(combination.defuzzify()));
                    variable->defuzzify();
                    CHECK_THAT(variable->getPreviousValue(), Approximates(combination.after().previous()));
                    CHECK_THAT(variable->getValue(), Approximates(combination.after().current()));
                }
            }
            return *this;
        }

        AssertOutputVariable& fsm_deprecated_v6(const std::vector<Combination>& combinations) {
            Constant dummy;
            for (auto& combination : combinations) {
                variable->fuzzyOutput()->clear();
                SECTION("Combination " + combination.number()) {
                    CAPTURE(combination.number());
                    variable->setPreviousValue(combination.before().previous());
                    variable->setValue(combination.before().current());
                    variable->setDefuzzifier(new MockDefuzzifier(combination.defuzzify()));
                    if (not Op::isNaN(combination.defuzzify()))
                        variable->fuzzyOutput()->setTerms({Activated(&dummy)});
                    variable->defuzzify_v6();
                    CHECK_THAT(variable->getValue(), Approximates(combination.after().current()));
                    CHECK_THAT(variable->getPreviousValue(), Approximates(combination.after().previous()));
                }
            }
            return *this;
        }
    };
}}

#endif
