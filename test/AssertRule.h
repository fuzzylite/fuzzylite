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

#ifndef FL_TEST_ASSERT_RULE_H
#define FL_TEST_ASSERT_RULE_H

#include "test/Headers.h"

namespace fuzzylite { namespace test {
    struct AssertAntecedent {
        FL_unique_ptr<Antecedent> antecedent;
        FL_unique_ptr<Engine> engine;

        AssertAntecedent(Antecedent* antecedent, std::unique_ptr<Engine> engine) :
            antecedent(antecedent),
            engine(std::move(engine)) {}

        AssertAntecedent& can_load_antecedent(const std::string& text) {
            antecedent->setText(text);
            antecedent->load(engine.get());
            CHECK(antecedent->isLoaded());
            return *this;
        }

        AssertAntecedent& cannot_load_antecedent(const std::string& text, const std::string& expectedException) {
            antecedent->setText(text);
            CHECK_THROWS_AS(antecedent->load(engine.get()), fl::Exception);
            CHECK_THROWS_WITH(antecedent->load(engine.get()), Catch::Matchers::StartsWith(expectedException));
            return *this;
        }

        AssertAntecedent& toInfix(const std::string& expected = "") {
            CHECK(antecedent->toInfix() == (expected.empty() ? antecedent->getText() : expected));
            return *this;
        }

        AssertAntecedent& toPrefix(const std::string& expected) {
            CHECK(antecedent->toPrefix() == expected);
            return *this;
        }

        AssertAntecedent& toPostfix(const std::string&& expected) {
            CHECK(antecedent->toPostfix() == expected);
            return *this;
        }

        AssertAntecedent& input_has_activation_degrees(
            const std::map<std::string, std::vector<fl::scalar>>& inputs,
            const std::map<std::string, std::vector<fl::scalar>>& rules,
            const TNorm* conjunction = fl::null,
            const SNorm* disjunction = fl::null
        ) {
            for (const auto& iv : inputs) {
                fl::InputVariable* input = engine->getInputVariable(iv.first);
                const auto& input_values = iv.second;

                for (std::size_t index = 0; index < input_values.size(); ++index) {
                    const auto& value = input_values.at(index);
                    input->setValue(value);

                    for (const auto& ra : rules) {
                        antecedent->setText(ra.first);
                        antecedent->load(engine.get());
                        const auto& obtained = antecedent->activationDegree(conjunction, disjunction);
                        const auto& expected = ra.second.at(index);
                        CAPTURE(iv.first, value, ra.first);
                        CHECK_THAT(obtained, Approximates(expected));
                    }
                }
            }
            return *this;
        }

        AssertAntecedent& output_has_activation_degrees(
            const std::map<std::string, std::vector<std::vector<fl::Activated>>>& outputs,
            const std::map<std::string, std::vector<fl::scalar>>& rules,
            const TNorm* conjunction = fl::null,
            const SNorm* disjunction = fl::null
        ) {
            for (const auto& ov : outputs) {
                fl::OutputVariable* output = engine->getOutputVariable(ov.first);
                const auto& output_values = ov.second;

                for (std::size_t index = 0; index < output_values.size(); ++index) {
                    const auto& value = output_values.at(index);
                    output->fuzzyOutput()->setTerms(value);

                    for (const auto& ra : rules) {
                        antecedent->setText(ra.first);
                        antecedent->load(engine.get());
                        const auto& obtained = antecedent->activationDegree(conjunction, disjunction);
                        const auto& expected = ra.second.at(index);
                        CAPTURE(ov.first, value, ra.first);
                        CHECK_THAT(obtained, Approximates(expected));
                    }
                }
            }
            return *this;
        }
    };

    struct AssertConsequent {
        FL_unique_ptr<Consequent> consequent;
        FL_unique_ptr<Engine> engine;

        AssertConsequent(Consequent* consequent, std::unique_ptr<Engine> engine) :
            consequent(consequent),
            engine(std::move(engine)) {}

        AssertConsequent& can_load_consequent(const std::string& text) {
            CAPTURE(text);
            consequent->setText(text);
            consequent->load(engine.get());
            CHECK(consequent->isLoaded());
            CHECK(not consequent->conclusions().empty());
            CHECK(consequent->toString() == text);
            return *this;
        }

        AssertConsequent& cannot_load_consequent(const std::string& text, const std::string& expectedException) {
            consequent->setText(text);
            CHECK_THROWS_AS(consequent->load(engine.get()), fl::Exception);
            CHECK_THROWS_WITH(consequent->load(engine.get()), Catch::Matchers::StartsWith(expectedException));
            return *this;
        }

        AssertConsequent& modify_consequent(
            const std::string& text,
            fl::scalar activation_degree,
            const std::map<std::string, std::vector<fl::Activated>>& expected,
            const TNorm* implication = fl::null
        ) {
            CAPTURE(text, activation_degree);
            CHECK(not expected.empty());

            consequent->setText(text);
            consequent->load(engine.get());
            consequent->modify(activation_degree, implication);

            for (const auto& pair : expected) {
                fl::OutputVariable* variable = engine->getOutputVariable(pair.first);

                std::vector<fl::Activated> non_zero;
                non_zero.reserve(variable->fuzzyOutput()->terms().size());
                for (const auto& activated : variable->fuzzyOutput()->terms())
                    if (Op::isGt(activated.getDegree(), 0.0))
                        non_zero.push_back(activated);

                const std::string& obtained_text
                    = Aggregated().aggregation(new Maximum).terms(non_zero).grouped().fuzzyValue();
                const std::string& expected_text
                    = Aggregated().aggregation(new Maximum).terms(pair.second).grouped().fuzzyValue();

                CAPTURE(variable->getName(), obtained_text, expected_text);
                CHECK(obtained_text == expected_text);

                variable->fuzzyOutput()->clear();
            }
            return *this;
        }
    };

    struct AssertRule {
        FL_unique_ptr<Rule> rule;
        Engine* engine;

        AssertRule(std::unique_ptr<Rule> rule, Engine* engine) : rule(std::move(rule)), engine(engine) {}

        AssertRule& is_enabled(bool enabled = true) {
            CHECK(rule->isEnabled() == enabled);
            return *this;
        }

        AssertRule& has_text(const std::string& text) {
            CHECK(rule->getText() == text);
            return *this;
        }

        AssertRule& has_weight(scalar weight) {
            CHECK_THAT(rule->getWeight(), Approximates(weight));
            return *this;
        }

        AssertRule& has_activation_degree(scalar degree) {
            CHECK_THAT(rule->getActivationDegree(), Approximates(degree));
            return *this;
        }

        AssertRule& is_triggered(bool triggered = true) {
            CHECK(rule->isTriggered() == triggered);
            return *this;
        }

        AssertRule& has_antecedent(const std::string& antedecent) {
            CHECK(rule->getAntecedent()->getText() == antedecent);
            return *this;
        }

        AssertRule& has_consequent(const std::string& consequent) {
            CHECK(rule->getConsequent()->getText() == consequent);
            return *this;
        }

        AssertRule& is_loaded(bool loaded = true) {
            CHECK(rule->isLoaded() == loaded);
            return *this;
        }

        AssertRule& can_parse_rule(const std::string& text, const std::string& expected = "") {
            FL_unique_ptr<Rule> test(Rule::parse(text, engine));
            CHECK(test->getText() == (expected.empty() ? text : expected));
            return *this;
        }
    };

}}
#endif
