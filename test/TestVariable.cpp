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
    struct VariableAssert {
        std::unique_ptr<Variable> variable;
        VariableAssert() = default;

        explicit VariableAssert(std::unique_ptr<Variable> actual) : variable(std::move(actual)) {}

        VariableAssert& has_name(const std::string& name) {
            CHECK(variable->getName() == name);
            return *this;
        }

        VariableAssert& has_description(const std::string& description) {
            CHECK(variable->getDescription() == description);
            return *this;
        }

        VariableAssert& is_enabled(bool enabled = true) {
            CHECK(variable->isEnabled() == enabled);
            return *this;
        }

        VariableAssert& has_range(scalar minimum, scalar maximum) {
            CHECK_THAT(variable->getMinimum(), Approximates(minimum));
            CHECK_THAT(variable->getMaximum(), Approximates(maximum));
            return *this;
        }

        VariableAssert& has_value(scalar value) {
            CHECK_THAT(variable->getValue(), Approximates(value));
            return *this;
        }

        VariableAssert& locks_value_in_range(bool locks = true) {
            CHECK(variable->isLockValueInRange() == locks);
            return *this;
        }

        VariableAssert& exports_fll(const std::string& fll) {
            CHECK(variable->toString() == fll);
            return *this;
        }

        VariableAssert& exports_fll(const std::vector<std::string>& expected) {
            auto obtained = Op::split(variable->toString(), "\n");
            for (std::size_t i = 0; i < obtained.size(); ++i)
                obtained[i] = Op::trim(obtained[i]);
            CHECK(obtained == expected);
            return *this;
        }

        VariableAssert& fuzzify(scalar value, const std::string& expected) {
            auto obtained = variable->fuzzify(value);
            CHECK(obtained == expected);
            return *this;
        }

        VariableAssert& fuzzy_values(std::vector<std::pair<scalar, std::string>> values_expected) {
            for (const auto& pair : values_expected) {
                const std::string& expected = pair.second;
                auto obtained = variable->fuzzify(pair.first);
                CHECK(obtained == expected);
            }
            return *this;
        }

        VariableAssert& highest_membership(scalar x, const Activated& expected) {
            scalar degree;
            Term* highest = variable->highestMembership(x, &degree);
            CHECK(highest == expected.getTerm());
            CHECK_THAT(degree, Approximates(expected.getDegree()));
            return *this;
        }

        VariableAssert& highest_activation(scalar x, const Activated& expected) {
            Activated highest = variable->highestActivation(x);
            CHECK(highest.getTerm() == expected.getTerm());
            CHECK_THAT(highest.getDegree(), Approximates(expected.getDegree()));
            highest_membership(x, expected);
            return *this;
        }

        VariableAssert& highest_activation(const std::vector<std::pair<scalar, Activated>>& highestActivations) {
            for (auto& pair : highestActivations)
                highest_activation(pair.first, pair.second);
            return *this;
        }

        VariableAssert& equals(const Variable& another) {
            CHECK(variable->getName() == another.getName());
            CHECK(variable->getDescription() == another.getDescription());
            CHECK(variable->isEnabled() == another.isEnabled());
            CHECK(variable->isLockValueInRange() == another.isLockValueInRange());

            CHECK_THAT(variable->getMinimum(), Approximates(another.getMinimum()));
            CHECK_THAT(variable->getMaximum(), Approximates(another.getMaximum()));
            CHECK_THAT(variable->getValue(), Approximates(another.getValue()));

            CHECK(variable->toString() == another.toString());
            return *this;
        }
    };

}}

namespace fuzzylite { namespace test { namespace variable {
    TEST_CASE("Default move/assign", "[!shouldfail]") {
        //@todo: support move in fuzzylite 8
        AssertConstructorOf<Variable>().supports_move();
    }

    TEST_CASE("Variable Constructors", "[variable][constructor]") {
        Variable test_variable{
            "Test",
            -1.0,
            1.0,  //
            {new Triangle{"triangle", 1.0, 2.0, 3.0, 0.5}}
        };
        test_variable.setDescription("A test variable");
        test_variable.setEnabled(false);
        test_variable.setValue(0.0);
        test_variable.setLockValueInRange(true);

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
                    "term: triangle Triangle 1.000 2.000 3.000 0.500"
                })
                .equals(test_variable);
        }

        SECTION("Copy Assignment") {
            Variable copy("copy", nan, nan, {new Constant()});
            copy = test_variable;
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
                .equals(test_variable);
        }

        SECTION("Clone") {
            VariableAssert(std::unique_ptr<Variable>(test_variable.clone())).equals(test_variable);
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
                {-1.0, Activated(null, nan)},
                {-0.75, Activated(low, 0.5)},
                {-0.50, Activated(low, 1.0)},
                {-0.25, Activated(low, 0.5)},
                {0.00, Activated(medium, 1.0)},
                {0.25, Activated(medium, 0.5)},
                {0.50, Activated(high, 1.0)},
                {0.75, Activated(high, 0.5)},
                {1.00, Activated(null, nan)},
                {nan, Activated(null, nan)},
                {inf, Activated(null, nan)},
                {inf, Activated(null, nan)},
            });
    }
}}}

namespace fuzzylite { namespace test { namespace input_variable {

}}}

namespace fuzzylite { namespace test { namespace output_variable {
}}}
