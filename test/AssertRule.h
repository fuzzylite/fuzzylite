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
}}
#endif
