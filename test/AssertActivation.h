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

#ifndef FL_TEST_ASSERT_ACTIVATION_H
#define FL_TEST_ASSERT_ACTIVATION_H

#include <catch2/catch_all.hpp>

#include "fuzzylite/Headers.h"
#include "test/Mock.h"

namespace fuzzylite { namespace test {
    struct AssertActivation {
        RuleBlock& _rule_block;

        explicit AssertActivation(RuleBlock& rule_block) : _rule_block(rule_block) {}

        AssertActivation& self() {
            return *this;
        }

        AssertActivation& activation(Activation* method) {
            _rule_block.setActivation(method);
            return self();
        }

        AssertActivation& exports_fll(const std::string& expected) {
            CHECK(FllExporter().toString(_rule_block.getActivation()) == expected);
            return self();
        }

        AssertActivation& is_configurable(const std::string& parameters = "") {
            Activation* method = _rule_block.getActivation();
            const std::string expected = method->toString();

            method->configure(method->parameters());
            const std::string obtained_reconfigured = method->toString();
            CHECK(expected == obtained_reconfigured);

            method->configure(parameters);
            const std::string obtained = method->toString();
            CHECK(expected == obtained);

            const std::unique_ptr<Activation> clone(method->clone());
            CHECK(expected == clone->toString());

            return self();
        }

        AssertActivation& fails_to_configure(const std::string& parameters, int required) {
            Activation* method = _rule_block.getActivation();
            std::ostringstream error;
            error << "[configuration error] activation <" << method->className() << "> requires <" << required
                  << "> parameters";
            CHECK_THROWS_MATCHES(
                method->configure(parameters),
                Exception,
                Catch::Matchers::MessageMatches(Catch::Matchers::StartsWith(error.str()))
            );
            return self();
        }

        AssertActivation& triggers(const std::vector<std::tuple<std::string, std::string, bool> >& variables_terms) {
            is_configurable();
            _rule_block.activate();
            for (const auto& expected : variables_terms) {
                std::string variable, term;
                bool triggered;
                std::tie(variable, term, triggered) = expected;
                bool found = false;
                for (auto* rule : _rule_block.rules()) {
                    auto mock_rule = dynamic_cast<MockRule*>(rule);
                    REQUIRE(mock_rule);
                    if (mock_rule->_variable == variable and mock_rule->_term == term) {
                        found = true;
                        CAPTURE(
                            variable, term, triggered, _rule_block.getName(), _rule_block.getActivation()->toString()
                        );
                        CHECK(mock_rule->isTriggered() == triggered);
                        break;
                    }
                }
                CAPTURE(variable, term, triggered);
                CAPTURE(_rule_block.toString());
                CHECK(found);
            }
            return self();
        }

        AssertActivation&
        triggers_with_degree(const std::vector<std::tuple<std::string, std::string, scalar> >& variables_terms) {
            is_configurable();
            _rule_block.activate();
            for (const auto& expected : variables_terms) {
                std::string variable, term;
                scalar degree;
                std::tie(variable, term, degree) = expected;
                bool found = false;
                for (auto* rule : _rule_block.rules()) {
                    auto mock_rule = dynamic_cast<MockRule*>(rule);
                    REQUIRE(mock_rule);
                    if (mock_rule->_variable == variable and mock_rule->_term == term) {
                        found = true;
                        CAPTURE(variable, term, degree, _rule_block.getName(), _rule_block.getActivation()->toString());
                        CHECK(mock_rule->isTriggered() == (degree > 0));
                        CHECK_THAT(mock_rule->getRuleActivationDegree(), Approximates(degree));
                        break;
                    }
                }
                CAPTURE(variable, term, degree);
                CAPTURE(_rule_block.toString());
                CHECK(found);
            }
            return self();
        }
    };
}}
#endif
