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
    // curiously recurring template pattern (CRTP): https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
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

    struct VariableAssert : Assert<VariableAssert, Variable> {
        using Assert::Assert;  // inherit constructors
    };

    struct InputVariableAssert : Assert<InputVariableAssert, InputVariable> {
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

        InputVariableAssert& equals(const InputVariable& another) override {
            return Assert::equals(another);
        }
    };

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

    struct OutputVariableAssert : Assert<OutputVariableAssert, OutputVariable> {
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

        OutputVariableAssert& equals(const OutputVariable& another) override {
            Assert::equals(another);
            CHECK_THAT(variable->getDefaultValue(), Approximates(another.getDefaultValue()));
            CHECK_THAT(variable->getPreviousValue(), Approximates(another.getPreviousValue()));
            CHECK(
                FllExporter().toString(variable->getDefuzzifier()) == FllExporter().toString(another.getDefuzzifier())
            );
            CHECK(variable->isLockPreviousValue() == another.isLockPreviousValue());
            return self();
        }

        OutputVariableAssert& fsm(const std::vector<Combination>& combinations) {
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

        OutputVariableAssert& fsm_deprecated_v6(const std::vector<Combination>& combinations) {
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

namespace fuzzylite { namespace test { namespace variable {

    Variable testVariable() {
        Variable variable{
            "Test",
            -1.0,
            1.0,  //
            {new Triangle{"triangle", 1.0, 2.0, 3.0, 0.5}}
        };
        variable.setDescription("A test variable");
        variable.setEnabled(false);
        variable.setValue(0.0);
        variable.setLockValueInRange(true);
        return variable;
    }

    TEST_CASE("Constructors", "[variable][constructor]") {
        SECTION("Default constructor") {
            VariableAssert(std::make_unique<Variable>())
                .has_name("")
                .has_description("")
                .is_enabled(true)
                .has_value(nan)
                .has_range(-inf, inf)
                .locks_value_in_range(false)
                .exports_fll(
                    std::vector<std::string>{"Variable: _", "enabled: true", "range: -inf inf", "lock-range: false"}
                );
        }
        SECTION("Copy constructor") {
            VariableAssert(std::make_unique<Variable>(testVariable()))
                .has_name("Test")
                .has_description("A test variable")
                .is_enabled(false)
                .has_value(0.0)
                .has_range(-1, 1)
                .locks_value_in_range(true)
                .exports_fll(std::vector<std::string>{
                    "Variable: Test",
                    "description: A test variable",
                    "enabled: false",
                    "range: -1.000 1.000",
                    "lock-range: true",
                    "term: triangle Triangle 1.000 2.000 3.000 0.500"
                })
                .equals(testVariable());
        }

        SECTION("Copy Assignment") {
            Variable copy("copy", nan, nan, {new Constant()});
            Variable test = testVariable();
            copy = test;
            VariableAssert(std::make_unique<Variable>(copy))
                .has_name("Test")
                .has_description("A test variable")
                .is_enabled(false)
                .has_value(0.0)
                .has_range(-1, 1)
                .locks_value_in_range(true)
                .exports_fll(std::vector<std::string>{
                    "Variable: Test",
                    "description: A test variable",
                    "enabled: false",
                    "range: -1.000 1.000",
                    "lock-range: true",
                    "term: triangle Triangle 1.000 2.000 3.000 0.500"
                })
                .equals(testVariable());
        }

        SECTION("Clone") {
            VariableAssert(std::make_unique<Variable>(testVariable())).can_clone();
        }
    }

    TEST_CASE("Variable getters and setters", "[variable][getters][setters]") {
        Variable test_variable;
        test_variable.setName("Test");
        test_variable.setDescription("A test variable");
        test_variable.setMinimum(-1.0);
        test_variable.setMaximum(1.0);
        test_variable.setEnabled(false);
        test_variable.setValue(0.0);
        test_variable.setLockValueInRange(true);
        test_variable.setTerms({new Constant{"pi", 3.1413}});

        SECTION("Check setters") {
            VariableAssert(std::make_unique<Variable>(test_variable))
                .has_name("Test")
                .has_description("A test variable")
                .is_enabled(false)
                .has_value(0.0)
                .has_range(-1, 1)
                .locks_value_in_range(true)
                .exports_fll(std::vector<std::string>{
                    "Variable: Test",
                    "description: A test variable",
                    "enabled: false",
                    "range: -1.000 1.000",
                    "lock-range: true",
                    "term: pi Constant 3.141"
                })
                .equals(test_variable);
        }

        SECTION("Add terms") {
            test_variable.addTerm(new Constant{"pix2", 6.282});
            CHECK(
                FllExporter().toString(test_variable.terms())
                == Op::join(
                    std::vector<std::string>{
                        "term: pi Constant 3.141",   // 1
                        "term: pix2 Constant 6.282"  // 2
                    },
                    "\n"
                )
            );
        }
        SECTION("Insert terms") {
            test_variable.addTerm(new Constant{"pix2", 6.282});
            test_variable.insertTerm(new Constant{"pix0", 0}, 0);
            test_variable.insertTerm(new Constant{"pix3", 9.423}, 3);
            CHECK(
                FllExporter().toString(test_variable.terms())
                == Op::join(
                    std::vector<std::string>{
                        "term: pix0 Constant 0.000",
                        "term: pi Constant 3.141",
                        "term: pix2 Constant 6.282",
                        "term: pix3 Constant 9.423"
                    },
                    "\n"
                )
            );
        }
        SECTION("Remove terms") {
            test_variable.addTerm(new Constant{"pix2", 6.282});
            test_variable.insertTerm(new Constant{"pix0", 0}, 0);
            test_variable.insertTerm(new Constant{"pix3", 9.423}, 3);
            test_variable.removeTerm(0);
            CHECK(
                FllExporter().toString(test_variable.terms())
                == Op::join(
                    std::vector<std::string>{
                        "term: pi Constant 3.141",  //
                        "term: pix2 Constant 6.282",
                        "term: pix3 Constant 9.423"
                    },
                    "\n"
                )
            );
            test_variable.removeTerm(1);
            CHECK(
                FllExporter().toString(test_variable.terms())
                == Op::join(
                    std::vector<std::string>{
                        "term: pi Constant 3.141",  //
                        "term: pix3 Constant 9.423"
                    },
                    "\n"
                )
            );
            test_variable.removeTerm(1);
            CHECK(
                FllExporter().toString(test_variable.terms())
                == Op::join(
                    std::vector<std::string>{
                        "term: pi Constant 3.141",
                    },
                    "\n"
                )
            );
        }
        SECTION("Has terms") {
            CHECK(test_variable.hasTerm("pi"));
            CHECK(not test_variable.hasTerm("pix"));
        }

        SECTION("Get terms") {
            CHECK_THROWS_MATCHES(
                test_variable.getTerm("X"),
                fl::Exception,
                Catch::Matchers::MessageMatches(
                    Catch::Matchers::StartsWith("[variable error] term <X> not found in variable <Test>")
                )
            );
        }
        SECTION("Type") {
            CHECK(test_variable.type() == Variable::Type::None);
        }
    }

    TEST_CASE("variable sorts terms", "[variable][terms][sort]") {
        SECTION("variable of Constant terms is sorted") {
            Variable variable("Variable", -10, 10);
            for (int i = 20; i >= 0; --i)
                variable.addTerm(new Constant(Op::str(i), i - 10));

            CHECK(variable.numberOfTerms() == 21);
            variable.sort();
            CHECK(variable.numberOfTerms() == 21);
            int value = -10;
            for (std::size_t i = 0; i < variable.terms().size(); ++i) {
                Constant* term = dynamic_cast<Constant*>(variable.terms().at(i));
                CHECK(term->getValue() == float(value));
                ++value;
            }
        }

        SECTION("variable of non-Constant terms is sorted") {
            Variable variable("Variable", -30, 30);
            for (int i = 20; i >= 0; --i)
                variable.addTerm(new Triangle(Op::str(i), i - 1, i, i + 1));
            CHECK(variable.numberOfTerms() == 21);
            variable.sort();
            CHECK(variable.numberOfTerms() == 21);
            int value = 0;
            for (std::size_t i = 0; i < variable.terms().size(); ++i) {
                Triangle* term = dynamic_cast<Triangle*>(variable.terms().at(i));
                CHECK(term->getName() == Op::str(value < 0 ? -1 * value : value));
                ++value;
            }
        }
    }

    TEST_CASE("Variable fuzzifies", "[variable]") {
        VariableAssert(std::make_unique<Variable>(
                           "Temperature",
                           0,
                           100,
                           std::vector<Term*>{
                               new Triangle("Low", -1.0, -1.0, 0.0),
                               new Triangle("Medium", -0.5, 0.0, 0.5),
                               new Triangle("High", 0.0, 1.0, 1.0)
                           }
                       ))
            .fuzzy_values({
                {-1.00, "1.000/Low + 0.000/Medium + 0.000/High"},
                {-0.50, "0.500/Low + 0.000/Medium + 0.000/High"},
                {-0.25, "0.250/Low + 0.500/Medium + 0.000/High"},
                {0.00, "0.000/Low + 1.000/Medium + 0.000/High"},
                {0.25, "0.000/Low + 0.500/Medium + 0.250/High"},
                {0.50, "0.000/Low + 0.000/Medium + 0.500/High"},
                {0.75, "0.000/Low + 0.000/Medium + 0.750/High"},
                {1.00, "0.000/Low + 0.000/Medium + 1.000/High"},
                //@todo: pyfuzzylite produces 0.000 instead of NaN
                {nan, "nan/Low + nan/Medium + nan/High"},
                {inf, "0.000/Low + 0.000/Medium + 0.000/High"},
                {-inf, "0.000/Low + 0.000/Medium + 0.000/High"},
            });
        VariableAssert(std::make_unique<Variable>(
                           "Temperature",
                           -inf,
                           inf,
                           std::vector<Term*>{
                               new Constant("A", -1),
                               new Constant("B", 0),
                               new Constant("C", 1),
                               new Constant("D", -inf),
                               new Constant("E", inf),
                               new Constant("F", nan),
                           }
                       ))
            //@todo: in pyfuzzylite, expected = "-1.000/A + 0.000/B + 1.000/C + 0.000/D + 1.000/E + 0.000/F"
            .fuzzify(nan, "-1.000/A + 0.000/B + 1.000/C - inf/D + inf/E + nan/F");
    }

    TEST_CASE("Variable finds highest membership terms", "[variable][terms]") {
        const auto low = new Triangle("Low", -1.0, -0.5, 0.0);
        const auto medium = new Triangle("Medium", -0.5, 0.0, 0.5);
        const auto high = new Triangle("High", 0.0, 0.5, 1.0);
        VariableAssert(std::make_unique<Variable>("name", -1.0, 1.0, std::vector<Term*>{low, medium, high}))
            .highest_activation({
                {-1.0, {}},
                {-0.75, {Activated(low, 0.5)}},
                {-0.50, {Activated(low, 1.0)}},
                {-0.25, {Activated(low, 0.5), Activated(medium, 0.5)}},
                {0.00, {Activated(medium, 1.0)}},
                {0.25, {Activated(medium, 0.5), Activated(high, 0.5)}},
                {0.50, {Activated(high, 1.0)}},
                {0.75, {Activated(high, 0.5)}},
                {1.00, {}},
                {nan, {}},
                {-inf, {}},
                {inf, {}},
            });
    }
}}}

namespace fuzzylite { namespace test { namespace input_variable {

    InputVariable testVariable() {
        InputVariable variable(
            "Test",
            -1.0,
            1.0,  //
            std::vector<Term*>{new Constant("A", 1), new Constant("B", 2)}
        );
        return variable;
    }

    TEST_CASE("InputVariable: Constructors", "[variable][input][constructor]") {
        SECTION("Default constructor") {
            InputVariableAssert(std::make_unique<InputVariable>())
                .has_name("")
                .has_description("")
                .is_enabled()
                .has_range(-inf, inf)
                .locks_value_in_range(false)
                .exports_fll(std::vector<std::string>{
                    "InputVariable: _", "enabled: true", "range: -inf inf", "lock-range: false"
                });
        }
        SECTION("Custom constructor") {
            InputVariableAssert(std::make_unique<InputVariable>(
                                    "Test",
                                    -1.0,
                                    1.0,  //
                                    std::vector<Term*>{new Constant("A", 1), new Constant("B", 2)}
                                ))
                .has_name("Test")
                .has_description("")
                .is_enabled()
                .has_range(-1, 1)
                .locks_value_in_range(false)
                .exports_fll(std::vector<std::string>{
                    {"InputVariable: Test",
                     "enabled: true",
                     "range: -1.000 1.000",
                     "lock-range: false",
                     "term: A Constant 1.000",
                     "term: B Constant 2.000"}
                });
        }
        SECTION("Copy constructor") {
            InputVariableAssert(std::make_unique<InputVariable>(testVariable()))
                .equals(testVariable())
                .exports_fll(std::vector<std::string>{
                    "InputVariable: Test",
                    "enabled: true",
                    "range: -1.000 1.000",
                    "lock-range: false",
                    "term: A Constant 1.000",
                    "term: B Constant 2.000"
                });
        }
        SECTION("Copy assign") {
            InputVariable copy;
            InputVariable test = testVariable();
            copy = test;
            InputVariableAssert(std::make_unique<InputVariable>(copy))
                .equals(testVariable())
                .exports_fll(std::vector<std::string>{
                    "InputVariable: Test",
                    "enabled: true",
                    "range: -1.000 1.000",
                    "lock-range: false",
                    "term: A Constant 1.000",
                    "term: B Constant 2.000"
                });
        }

        SECTION("Clone") {
            InputVariableAssert(std::make_unique<InputVariable>(testVariable())).can_clone();
        }
        SECTION("Type") {
            CHECK(InputVariable().type() == Variable::Input);
        }
    }

    TEST_CASE("InputVariable: fuzzy values") {
        InputVariableAssert(std::make_unique<InputVariable>(
                                "name",
                                -1.0,
                                1.0,
                                std::vector<Term*>{
                                    new Triangle{"Low", -1.0, -1.0, 0.0},
                                    new Triangle{"Medium", -0.5, 0.0, 0.5},
                                    new Triangle{"High", 0.0, 1.0, 1.0},
                                }
                            ))
            .has_fuzzy_values({
                {-1.00, "1.000/Low + 0.000/Medium + 0.000/High"},
                {-0.50, "0.500/Low + 0.000/Medium + 0.000/High"},
                {-0.25, "0.250/Low + 0.500/Medium + 0.000/High"},
                {0.00, "0.000/Low + 1.000/Medium + 0.000/High"},
                {0.25, "0.000/Low + 0.500/Medium + 0.250/High"},
                {0.50, "0.000/Low + 0.000/Medium + 0.500/High"},
                {0.75, "0.000/Low + 0.000/Medium + 0.750/High"},
                {1.00, "0.000/Low + 0.000/Medium + 1.000/High"},
                {nan, "nan/Low + nan/Medium + nan/High"},  // @todo: pyfuzzylite produces 0 instead of nan
                {inf, "0.000/Low + 0.000/Medium + 0.000/High"},
                {inf, "0.000/Low + 0.000/Medium + 0.000/High"},
            });
    }

}}}

namespace fuzzylite { namespace test { namespace output_variable {

    OutputVariable testVariable() {
        OutputVariable variable(
            "Test",
            -1.0,
            1.0,  //
            std::vector<Term*>{new Constant("A", 1), new Constant("B", 2)}
        );
        variable.setDefuzzifier(new Centroid);
        variable.setAggregation(new Maximum);
        variable.setDefaultValue(0);
        variable.setPreviousValue(-1);
        variable.setLockPreviousValue(true);
        return variable;
    }

    TEST_CASE("OutputVariable: Constructors", "[variable][output][constructor]") {
        SECTION("Default constructor") {
            OutputVariableAssert(std::make_unique<OutputVariable>())
                .has_name("")
                .has_description("")
                .is_enabled()
                .has_range(-inf, inf)
                .locks_value_in_range(false)
                .has_previous_value(nan)
                .has_default_value(nan)
                .locks_previous_value(false)
                .has_aggregation("none")
                .exports_fll(std::vector<std::string>{
                    "OutputVariable: _",
                    "enabled: true",
                    "range: -inf inf",
                    "lock-range: false",
                    "aggregation: none",
                    "defuzzifier: none",
                    "default: nan",
                    "lock-previous: false"
                });
        }
        SECTION("Custom constructor") {
            OutputVariableAssert(std::make_unique<OutputVariable>(
                                     "Test",
                                     -1.0,
                                     1.0,  //
                                     std::vector<Term*>{new Constant("A", 1), new Constant("B", 2)}
                                 ))
                .has_name("Test")
                .has_description("")
                .is_enabled()
                .has_range(-1, 1)
                .locks_value_in_range(false)
                .has_previous_value(nan)
                .has_default_value(nan)
                .locks_previous_value(false)
                .has_aggregation("none")
                .exports_fll(std::vector<std::string>{
                    {"OutputVariable: Test",
                     "enabled: true",
                     "range: -1.000 1.000",
                     "lock-range: false",
                     "aggregation: none",
                     "defuzzifier: none",
                     "default: nan",
                     "lock-previous: false",
                     "term: A Constant 1.000",
                     "term: B Constant 2.000"}
                });
        }

        // OutputVariable
        SECTION("Copy constructor") {
            OutputVariableAssert(std::make_unique<OutputVariable>(testVariable()))
                .equals(testVariable())
                .exports_fll(std::vector<std::string>{
                    "OutputVariable: Test",
                    "enabled: true",
                    "range: -1.000 1.000",
                    "lock-range: false",
                    "aggregation: Maximum",
                    "defuzzifier: Centroid",
                    "default: 0.000",
                    "lock-previous: true",
                    "term: A Constant 1.000",
                    "term: B Constant 2.000"
                });
        }
        SECTION("Copy assign") {
            OutputVariable copy;
            OutputVariable test = testVariable();
            copy = test;
            OutputVariableAssert(std::make_unique<OutputVariable>(copy))
                .equals(testVariable())
                .exports_fll(std::vector<std::string>{
                    "OutputVariable: Test",
                    "enabled: true",
                    "range: -1.000 1.000",
                    "lock-range: false",
                    "aggregation: Maximum",
                    "defuzzifier: Centroid",
                    "default: 0.000",
                    "lock-previous: true",
                    "term: A Constant 1.000",
                    "term: B Constant 2.000"
                });
        }

        SECTION("Clone") {
            OutputVariableAssert(std::make_unique<OutputVariable>(testVariable())).can_clone();
        }
        SECTION("Type") {
            CHECK(OutputVariable().type() == Variable::Output);
        }
    }

    TEST_CASE("Output Variable: fuzzy output values", "[variable][output][fuzzy]") {
        const auto low = new Triangle("Low", -1.0, -0.5, 0.0);
        const auto medium = new Triangle("Medium", -0.5, 0.0, 0.5);
        const auto high = new Triangle("High", 0.0, 0.5, 1.0);
        OutputVariableAssert assert_that(
            std::make_unique<OutputVariable>("test", -1, 1, std::vector<Term*>{low, medium, high})
        );
        assert_that.when_fuzzy_output_is({}).has_fuzzy_value("0.000/Low + 0.000/Medium + 0.000/High");
        assert_that.when_fuzzy_output_is({Activated(low, 0.5)})
            .has_fuzzy_value("0.500/Low + 0.000/Medium + 0.000/High");
        assert_that.when_fuzzy_output_is({Activated(low, -1.0), Activated(medium, -0.5), Activated(high, -0.1)})
            .has_fuzzy_value("-1.000/Low - 0.500/Medium - 0.100/High");
    }

    TEST_CASE("Output variable: Clear", "[variable][output][clear]") {
        const auto low = new Triangle("Low", -1.0, -0.5, 0.0);
        const auto medium = new Triangle("Medium", -0.5, 0.0, 0.5);
        const auto high = new Triangle("High", 0.0, 0.5, 1.0);
        OutputVariableAssert assert_that(
            std::make_unique<OutputVariable>("test", -1, 1, std::vector<Term*>{low, medium, high})
        );
        assert_that.variable->setValue(0.0);
        assert_that.variable->setPreviousValue(-1.0);
        assert_that.variable->fuzzyOutput()->terms({Activated(low, 0.5), Activated(medium, 0.5), Activated(high, 0.5)});

        // CHECKS
        assert_that.has_value(0.0).has_previous_value(-1.0).has_fuzzy_value("0.500/Low + 0.500/Medium + 0.500/High");

        // CLEAR
        assert_that.variable->clear();

        // CHECKS
        assert_that.has_value(nan).has_previous_value(nan).has_fuzzy_value("0.000/Low + 0.000/Medium + 0.000/High");
    }

    TEST_CASE("OutputVariable::defuzzify behavior", "[variable][output][defuzzify]") {
        OutputVariable variable;
        variable.setName("test");

        SECTION("Returns early if disabled") {
            variable.setEnabled(false);
            variable.setValue(42.0);
            variable.defuzzify();
            CHECK(variable.getValue() == 42.0);  // Value remains unchanged
        }

        SECTION("Throws exception if no defuzzifier is set") {
            variable.setEnabled(true);

            CHECK_THROWS_MATCHES(
                variable.defuzzify(),
                Exception,
                Catch::Matchers::MessageMatches(Catch::Matchers::StartsWith(
                    "[defuzzify error] expected a defuzzifier in variable 'test', but got null"
                ))
            );
        }

        SECTION("Uses defuzzifier to compute new value") {
            variable.setEnabled(true);
            variable.setDefuzzifier(new MockDefuzzifier(5.5));

            variable.defuzzify();
            CHECK_THAT(variable.getValue(), Approximates(5.5));
        }

        SECTION("Locks previous value if required and result is NaN") {
            variable.setEnabled(true);
            variable.setLockPreviousValue(true);
            variable.setPreviousValue(7.7);
            variable.setDefuzzifier(new MockDefuzzifier(nan));

            variable.defuzzify();
            CHECK_THAT(variable.getValue(), Approximates(7.7));  // Uses previous value
        }

        SECTION("Uses default value if result is NaN") {
            variable.setEnabled(true);
            variable.setLockPreviousValue(false);
            variable.setDefaultValue(3.3);
            variable.setDefuzzifier(new MockDefuzzifier(nan));

            variable.defuzzify();
            CHECK_THAT(variable.getValue(), Approximates(3.3));  // Uses default value
        }

        SECTION("Updates previous value correctly") {
            variable.setEnabled(true);
            variable.setValue(10.0);
            variable.setDefuzzifier(new MockDefuzzifier(20.0));

            variable.defuzzify();
            CHECK_THAT(variable.getPreviousValue(), Approximates(10.0));  // Previous value updated
            CHECK_THAT(variable.getValue(), Approximates(20.0));          // New value set
        }
    }

    TEST_CASE("Defuzzifification State Machine", "[output][variable][defuzzify][fsm]") {
        OutputVariableAssert assert_that(std::make_unique<OutputVariable>("test", -1.0, 1.0));
        SECTION("(a) When default is nan and no locks, value is defuzzified and previous value is updated") {
            const scalar def = nan;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(false);
            assert_that.variable->setLockPreviousValue(false);

            assert_that.fsm({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(inf).previous(inf),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(inf).previous(inf),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(inf).previous(inf),
            });
        }
        SECTION("(b) When default is not nan and no locks, value is defuzzified, default is used, and previous "
                "value "
                "is updated") {
            const scalar def = 0.7;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(false);
            assert_that.variable->setLockPreviousValue(false);

            assert_that.fsm({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(inf).previous(inf),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(inf).previous(inf),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(inf).previous(inf),
            });
        }

        SECTION("(c) When default is higher than the range, and locks previous and value, then default is "
                "capped when used"
                "used and stored in previous") {
            const scalar def = 7.7;
            const scalar def_cap = 1.0;
            const scalar cap = 1.0;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(true);
            assert_that.variable->setLockPreviousValue(true);

            assert_that.fsm({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(0.0).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(0.0).previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(cap).previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(0.0).previous(cap),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(cap).previous(cap),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(def_cap).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def_cap).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(cap).previous(nan),
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(cap).previous(cap),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(def_cap).previous(0.0),
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(def_cap).previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(cap).previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(cap).previous(cap),
            });
        }

        SECTION("(d) When default is higher than the range, and locks value in range, then default is capped when "
                "used and stored in previous") {
            const scalar def = 7.7;
            const scalar def_cap = 1.0;
            const scalar cap = 1.0;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(true);
            assert_that.variable->setLockPreviousValue(false);

            assert_that.fsm({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(def_cap).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(def_cap).previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(cap).previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(cap).previous(cap),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(def_cap).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def_cap).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(cap).previous(nan),
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(cap).previous(cap),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(def_cap).previous(0.0),
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(def_cap).previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(cap).previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(cap).previous(cap),
            });
        }

        SECTION("(e) When default is higher than the range, and locks previous value, "
                "previous is used when defuzzification is nan"
                "default is used when previous is nan") {
            const scalar def = 7.7;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(false);
            assert_that.variable->setLockPreviousValue(true);

            assert_that.fsm({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(0.0).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(0.0).previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(0.0).previous(inf),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(inf).previous(inf),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(inf).previous(inf),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(inf).previous(0.0),
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(inf).previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(inf).previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(inf).previous(inf),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(inf).previous(inf),
            });
        }
    }

    TEST_CASE("Defuzzifification State Machine v6", "[output][variable][defuzzify][fsm][v6]") {
        OutputVariableAssert assert_that(std::make_unique<OutputVariable>("test", -1.0, 1.0));
        SECTION("(a) When default is nan and no locks, value is defuzzified and previous value is updated") {
            const scalar def = nan;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(false);
            assert_that.variable->setLockPreviousValue(false);

            assert_that.fsm_deprecated_v6({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                // previous values won't update to non-finite current values
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(0.0
                ),  //.previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(def).previous(0.0
                ),  //.previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(inf).previous(0.0
                ),  //.previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(0.0
                ),  //.previous(inf),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(def).previous(0.0
                ),  //.previous(inf),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(inf).previous(0.0
                ),  //.previous(inf),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(inf).previous(nan),
                // previous values won't update to non-finite current values
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(nan
                ),  //.previous(inf),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def).previous(nan
                ),  //.previous(inf),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(inf).previous(nan
                ),  //.previous(inf),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                // previous values won't update to non-finite current values
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(inf
                ),  //.previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(def).previous(inf
                ),  //.previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(inf).previous(inf
                ),  //.previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(inf).previous(inf),
            });
        }
        SECTION("(b) When default is not nan and no locks, value is defuzzified, default is used, and previous "
                "value is updated") {
            const scalar def = 0.7;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(false);
            assert_that.variable->setLockPreviousValue(false);

            assert_that.fsm_deprecated_v6({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                // previous values won't update to non-finite values
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(0.0
                ),  //.previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(def).previous(0.0
                ),  //.previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(inf).previous(0.0
                ),  //.previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(0.0
                ),  //.previous(inf),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(def).previous(0.0
                ),  //.previous(inf),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(inf).previous(0.0
                ),  //.previous(inf),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(inf).previous(nan),

                // previous values won't update to non-finite current values
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(nan
                ),  //.previous(inf),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def).previous(nan
                ),  //.previous(inf),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(inf).previous(nan
                ),  //.previous(inf),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(def).previous(0.0),
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                // previous values won't update to non-finite current values
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(inf
                ),  //.previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(def).previous(inf
                ),  //.previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(inf).previous(inf
                ),  //.previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(def).previous(inf),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(inf).previous(inf),
            });
        }

        SECTION("(c) When default is higher than the range, and locks previous and value, then default is "
                "capped when used"
                "used and stored in previous") {
            const scalar def = 7.7;
            const scalar def_cap = 1.0;
            const scalar cap = 1.0;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(true);
            assert_that.variable->setLockPreviousValue(true);

            assert_that.fsm_deprecated_v6({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(0.0).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                // previous values won't update to non-finite current values
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(0.0
                ),  //.previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(0.0).previous(0.0
                ),  //.previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(cap).previous(0.0
                ),  //.previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                // current = cap (won't change), previous = cap because updates
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(cap).previous(cap
                ),  // current(0.0),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(cap).previous(cap),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(0.0).previous(0.0
                ),  // current(def_cap).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def_cap).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(cap).previous(nan),
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(cap).previous(cap),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(0.0).previous(0.0
                ),  // current(def_cap)
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(inf
                ),  //.previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(def_cap).previous(inf
                ),  //.previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(cap).previous(inf
                ),  //.previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(cap).previous(cap),
            });
        }

        SECTION("(d) When default is higher than the range, and locks value in range, then default is capped when "
                "used and stored in previous") {
            const scalar def = 7.7;
            const scalar def_cap = 1.0;
            const scalar cap = 1.0;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(true);
            assert_that.variable->setLockPreviousValue(false);

            assert_that.fsm_deprecated_v6({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(def_cap).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(0.0
                ),  //.previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(def_cap).previous(0.0
                ),  //.previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(cap).previous(0.0
                ),  //.previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(cap).previous(cap),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(def_cap).previous(0.0),
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def_cap).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(cap).previous(nan),
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(cap).previous(cap),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(def_cap).previous(0.0),
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(cap).previous(0.0),
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(inf
                ),  //.previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(def_cap).previous(inf
                ),  //.previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(cap).previous(inf
                ),  //.previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(cap),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(def_cap).previous(cap),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(cap).previous(cap),
            });
        }

        SECTION("(e) When default is higher than the range, and locks previous value, "
                "previous is used when defuzzification is nan"
                "default is used when previous is nan") {
            const scalar def = 7.7;
            assert_that.variable->setDefaultValue(def);
            assert_that.variable->setLockValueInRange(false);
            assert_that.variable->setLockPreviousValue(true);

            assert_that.fsm_deprecated_v6({
                Combination("01").when().previous(0.0).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("02").when().previous(0.0).current(0.0).defuzzify(nan).current(0.0).previous(0.0),
                Combination("03").when().previous(0.0).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                // previous value won't update to non-finite current value
                Combination("04").when().previous(0.0).current(nan).defuzzify(0.0).current(0.0).previous(0.0
                ),  //.previous(nan),
                Combination("05").when().previous(0.0).current(nan).defuzzify(nan).current(0.0).previous(0.0
                ),  //.previous(nan),
                Combination("06").when().previous(0.0).current(nan).defuzzify(inf).current(inf).previous(0.0
                ),  //.previous(nan),
                Combination("07").when().previous(0.0).current(inf).defuzzify(0.0).current(0.0).previous(0.0
                ),  //.previous(inf),
                Combination("08").when().previous(0.0).current(inf).defuzzify(nan).current(0.0).previous(0.0
                ),  //.previous(inf),
                Combination("09").when().previous(0.0).current(inf).defuzzify(inf).current(inf).previous(0.0
                ),  //.previous(inf),

                Combination("10").when().previous(nan).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("11").when().previous(nan).current(0.0).defuzzify(nan).current(0.0).previous(0.0
                ),  //.current(def)
                Combination("12").when().previous(nan).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                Combination("13").when().previous(nan).current(nan).defuzzify(0.0).current(0.0).previous(nan),
                Combination("14").when().previous(nan).current(nan).defuzzify(nan).current(def).previous(nan),
                Combination("15").when().previous(nan).current(nan).defuzzify(inf).current(inf).previous(nan),
                // previous value won't update to non-finite current value
                Combination("16").when().previous(nan).current(inf).defuzzify(0.0).current(0.0).previous(nan
                ),  //.previous(inf),
                Combination("17").when().previous(nan).current(inf).defuzzify(nan).current(def).previous(nan
                ),  //.previous(inf),
                Combination("18").when().previous(nan).current(inf).defuzzify(inf).current(inf).previous(nan
                ),  //.previous(inf),

                Combination("19").when().previous(inf).current(0.0).defuzzify(0.0).current(0.0).previous(0.0),
                Combination("20").when().previous(inf).current(0.0).defuzzify(nan).current(0.0).previous(0.0
                ),  //.current(inf)
                Combination("21").when().previous(inf).current(0.0).defuzzify(inf).current(inf).previous(0.0),
                // previous value won't update to non-finite current value
                Combination("22").when().previous(inf).current(nan).defuzzify(0.0).current(0.0).previous(inf
                ),  //.previous(nan),
                Combination("23").when().previous(inf).current(nan).defuzzify(nan).current(inf).previous(inf
                ),  //.previous(nan),
                Combination("24").when().previous(inf).current(nan).defuzzify(inf).current(inf).previous(inf
                ),  //.previous(nan),
                Combination("25").when().previous(inf).current(inf).defuzzify(0.0).current(0.0).previous(inf),
                Combination("26").when().previous(inf).current(inf).defuzzify(nan).current(inf).previous(inf),
                Combination("27").when().previous(inf).current(inf).defuzzify(inf).current(inf).previous(inf),
            });
        }
    }
}}}
