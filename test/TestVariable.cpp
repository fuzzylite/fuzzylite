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
        std::shared_ptr<Variable> variable;
        VariableAssert() = default;

        explicit VariableAssert(std::shared_ptr<Variable> shared) : variable(std::move(shared)) {}

        VariableAssert& has_name(const std::string& name) {
            CHECK(variable->getName() == name);
            return *this;
        }

        VariableAssert& has_description(const std::string& description) {
            CHECK(variable->getDescription() == description);
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
    };

    TEST_CASE("Variable", "[variable]") {
        SECTION("Constructor") {
            auto compare = &Variable::toString;
            // AssertConstructor().can_copy_and_move(Variable(), compare);
            auto v = new Variable();
            AssertConstructor().can_copy_and_move(*v, compare);
            // AssertConstructor().can_copy_and_move(
            //     Variable(
            //         "Test variable",
            //         -1.0,
            //         1.0,  //
            //         {new Triangle{"triangle", 1.0, 2.0, 3.0, 0.5}}
            //     ),
            //     compare
            // );

            VariableAssert(std::make_shared<Variable>())
                .has_name("")
                .has_description("")
                .exports_fll(Op::join(
                    std::vector<std::string>{
                        "Variable: _",
                        "enabled: true",
                        "range: -inf inf",
                        "lock-range: false",
                    },
                    "\n  "
                ));
        }
        SECTION("Constructor") {
            VariableAssert(std::make_shared<Variable>("variable", -1.0, 1.0))
                .has_name("variable")
                .has_description("")
                .exports_fll({"Variable: variable", "enabled: true", "range: -1.000 1.000", "lock-range: false"});
        }
        SECTION("Copy constructor") {
            Variable variable("variable", -1.0, 1.0);
            variable.setDescription("description");
            variable.setEnabled(false);
            variable.setTerms({new Triangle("triangle")});
            variable.setLockValueInRange(true);

            VariableAssert(std::make_unique<Variable>(variable))
                .has_name("variable")
                .has_description("description")
                .exports_fll(
                    {"Variable: variable",
                     "description: description",
                     "enabled: false",
                     "range: -1.000 1.000",
                     "lock-range: true",
                     "term: triangle Triangle nan nan nan"}
                );
        }
        SECTION("Copy assignment") {
            Variable variable("variable", -1.0, 1.0);
            variable.setDescription("description");
            variable.setEnabled(false);
            variable.setTerms({new Triangle("triangle")});
            variable.setLockValueInRange(true);

            Variable copy;
            copy = variable;
            VariableAssert(std::make_unique<Variable>(copy))
                .has_name("variable")
                .has_description("description")
                .exports_fll(
                    {"Variable: variable",
                     "description: description",
                     "enabled: false",
                     "range: -1.000 1.000",
                     "lock-range: true",
                     "term: triangle Triangle nan nan nan"}
                );
        }
        SECTION("Copy assignment") {
            Variable variable("variable", -1.0, 1.0);
            variable.setDescription("description");
            variable.setEnabled(false);
            variable.setTerms({new Triangle("triangle")});
            variable.setLockValueInRange(true);

            Variable copy = std::move(variable);
            CHECK(not variable.toString().empty());
            VariableAssert(std::make_unique<Variable>(copy))
                .has_name("variable")
                .has_description("description")
                .exports_fll(
                    {"Variable: variable",
                     "description: description",
                     "enabled: false",
                     "range: -1.000 1.000",
                     "lock-range: true",
                     "term: triangle Triangle nan nan nan"}
                );
        }
    }

    TEST_CASE("variable of Constant terms is sorted", "[variable][variable]") {
        Variable variable("Variable", -10, 10);
        for (int i = 20; i >= 0; --i)
            variable.addTerm(new Constant(Op::str(i), i - 10));

        FL_DBG(variable.toString());
        REQUIRE(variable.numberOfTerms() == 21);
        variable.sort();
        REQUIRE(variable.numberOfTerms() == 21);
        int value = -10;
        for (std::size_t i = 0; i < variable.terms().size(); ++i) {
            Constant* term = dynamic_cast<Constant*>(variable.terms().at(i));
            REQUIRE(term);
            REQUIRE(term->getValue() == float(value));
            ++value;
        }
        FL_DBG(variable.toString());
    }

    TEST_CASE("variable of Triangle terms is sorted", "[variable][variable]") {
        Variable variable("Variable", -30, 30);
        for (int i = 20; i >= 0; --i)
            variable.addTerm(new Triangle(Op::str(i), i - 1, i, i + 1));
        FL_DBG(variable.toString());
        REQUIRE(variable.numberOfTerms() == 21);
        variable.sort();
        FL_DBG("Shuffled:");
        FL_DBG(variable.toString());
        REQUIRE(variable.numberOfTerms() == 21);
        int value = 0;
        for (std::size_t i = 0; i < variable.terms().size(); ++i) {
            Triangle* term = dynamic_cast<Triangle*>(variable.terms().at(i));
            REQUIRE(term);
            REQUIRE(term->getName() == Op::str(value < 0 ? -1 * value : value));
            ++value;
        }
        FL_DBG(variable.toString());
    }

}};
