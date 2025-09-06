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

        AssertAntecedent(Antecedent* antecedent, Engine* engine) : antecedent(antecedent), engine(engine) {}

        AssertAntecedent& can_load_antecedent_postfix(const std::string& text, const std::string& expected) {
            antecedent->setText(text);
            antecedent->load(engine.get());
            CHECK(antecedent->isLoaded());
            CHECK(antecedent->toPostfix() == expected);
            return *this;
        }

        AssertAntecedent& can_load_antecedent_prefix(const std::string& text, const std::string& expected) {
            antecedent->setText(text);
            antecedent->load(engine.get());
            CHECK(antecedent->isLoaded());
            CHECK(antecedent->toPrefix() == expected);
            return *this;
        }

        AssertAntecedent& can_load_antecedent(const std::string& text) {
            antecedent->setText(text);
            antecedent->load(engine.get());
            CHECK(antecedent->isLoaded());
            CHECK(antecedent->toInfix() == text);
            return *this;
        }

        AssertAntecedent& cannot_load_antecedent(const std::string& text, const std::string& expectedException) {
            antecedent->setText(text);
            CHECK_THROWS_AS(antecedent->load(engine.get()), fl::Exception);
            CHECK_THROWS_WITH(antecedent->load(engine.get()), Catch::Matchers::StartsWith(expectedException));
            return *this;
        }

        AssertAntecedent& has_activation_degrees() {
            return *this;
        }
    };
}}
#endif
