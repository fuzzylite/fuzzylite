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

#include "test/AssertRule.h"
#include "test/Headers.h"

namespace fuzzylite { namespace test {
    TEST_CASE("Rule: Expression: Proposition", "[rule][expression][proposition]") {
        SECTION("Empty proposition") {
            CHECK(Proposition().toString().empty());
        }
        SECTION("Proposition with Variable") {
            Variable variable("variable");
            Constant term("term");

            Proposition proposition(&variable, {new Very}, &term);
            CHECK(proposition.toString() == "variable is very term");
        }
    }

    TEST_CASE("Rule: Expression: Operators", "[rule][expression][operator]") {
        SECTION("Empty operator") {
            CHECK(fl::Operator().name.empty());
        }
        SECTION("Operator without operands") {
            CHECK(fl::Operator("OR").toString() == "OR");
        }
        SECTION("Operator w./ith operands") {
            Variable variable("variable");
            Constant term("term");

            Proposition* left = new Proposition(&variable, {}, &term);
            Proposition* right = new Proposition(&variable, {}, &term);
            CHECK(fl::Operator("AND", left, right).toString() == "AND");
        }
    }

    std::string simple_dimmer() {
        return fl::Op::join(
            std::vector<std::string>{
                "Engine: SimpleDimmer",
                "InputVariable: Ambient",
                "  enabled: true",
                "  range: 0.000 1.000",
                "  lock-range: false",
                "  term: DARK Triangle 0.000 0.250 0.500",
                "  term: MEDIUM Triangle 0.250 0.500 0.750",
                "  term: BRIGHT Triangle 0.500 0.750 1.000",
                "OutputVariable: Power",
                "  enabled: true",
                "  range: 0.000 1.000",
                "  lock-range: false",
                "  aggregation: Maximum",
                "  defuzzifier: Centroid 200",
                "  default: nan",
                "  lock-previous: false",
                "  term: LOW Triangle 0.000 0.250 0.500",
                "  term: MEDIUM Triangle 0.250 0.500 0.750",
                "  term: HIGH Triangle 0.500 0.750 1.000"
            },
            "\n"
        );
    }

    FL_unique_ptr<Engine> engine(FllImporter().fromString(simple_dimmer()));

    TEST_CASE("Rule/Load/Antecedent", "[rule][antecedent]") {
        Antecedent antecedent;
        CHECK(not antecedent.isLoaded());

        antecedent.setExpression(new Proposition());
        CHECK(antecedent.isLoaded());

        antecedent.unload();
        CHECK(not antecedent.isLoaded());
        CHECK(not antecedent.getExpression());
    }

    TEST_CASE("Rule/Load/Antecedent/input variable", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Ambient is DARK")
            .toInfix()
            .can_load_antecedent("Ambient is very DARK")
            .toInfix()
            .can_load_antecedent("Ambient is any")
            .toInfix();
    }

    TEST_CASE("Rule/Load/Antecedent/output variable", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Power is HIGH")
            .toInfix()
            .can_load_antecedent("Power is very HIGH")
            .toInfix()
            .can_load_antecedent("Power is any")
            .toInfix();
    }

    TEST_CASE("Rule/Load/Antecedent/input variable/connectors", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Ambient is DARK and Ambient is BRIGHT")
            .toInfix()
            .toPostfix("Ambient is DARK Ambient is BRIGHT and")
            .can_load_antecedent("Ambient is very DARK or Ambient is very BRIGHT")
            .toInfix()
            .toPostfix("Ambient is very DARK Ambient is very BRIGHT or")
            .can_load_antecedent("Ambient is any and Ambient is not any")
            .toInfix()
            .toPostfix("Ambient is any Ambient is not any and");
    }

    TEST_CASE("Rule/Load/Antecedent/output variable/connectors", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Power is HIGH and Power is LOW")
            .toInfix()
            .toPostfix("Power is HIGH Power is LOW and")
            .can_load_antecedent("Power is very HIGH or Power is very LOW")
            .toInfix()
            .toPostfix("Power is very HIGH Power is very LOW or")
            .can_load_antecedent("Power is any and Power is not any")
            .toInfix()
            .toPostfix("Power is any Power is not any and");
    }

    TEST_CASE("Rule/Load/Antecedent/Fails", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .cannot_load_antecedent("", "[syntax error] antecedent is empty")
            .cannot_load_antecedent(
                "Ambient is any and",
                "[syntax error] logical operator <and> expects two operands, but found <1> in antecedent"
            )
            .cannot_load_antecedent(
                "Ambient is any DARK",
                "[syntax error] antecedent expected variable or logical operator, but found <DARK>"
            )
            .cannot_load_antecedent(
                "InvalidVariable is any",
                "[syntax error] antecedent expected variable or logical operator, but found <InvalidVariable>"
            )
            .cannot_load_antecedent(
                "Ambient isn't", "[syntax error] antecedent expected keyword <is>, but found <isn't>"
            )
            .cannot_load_antecedent(
                "Ambient is very invalid", "[syntax error] antecedent expected hedge or term, but found <invalid>"
            )
            .cannot_load_antecedent(
                "Ambient is invalid_term", "[syntax error] antecedent expected hedge or term, but found <invalid_term>"
            );
    }

    TEST_CASE("Rule/Antecedent/Text", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Ambient is DARK and Ambient is BRIGHT")
            .toInfix()
            .toPostfix("Ambient is DARK Ambient is BRIGHT and")
            .toPrefix("and Ambient is DARK Ambient is BRIGHT");
    }

    TEST_CASE("Rule/Antecedent/Text/Group", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Ambient is DARK and (Ambient is MEDIUM or Ambient is BRIGHT)")
            .toInfix("Ambient is DARK and Ambient is MEDIUM or Ambient is BRIGHT")
            .toPostfix("Ambient is DARK Ambient is MEDIUM Ambient is BRIGHT or and")
            .toPrefix("and Ambient is DARK or Ambient is MEDIUM Ambient is BRIGHT");
    }

    TEST_CASE("Rule/Antecedent/Text/Group/Implicit", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Ambient is BRIGHT or Ambient is DARK and Ambient is MEDIUM")
            .toInfix()
            .toPostfix("Ambient is BRIGHT Ambient is DARK Ambient is MEDIUM and or")
            .toPrefix("or Ambient is BRIGHT and Ambient is DARK Ambient is MEDIUM");
    }

    TEST_CASE("Rule/Antecedent/ActivationDegrees/DisabledVariable", "[rule][antecedent]") {
        engine->getInputVariable("Ambient")->setEnabled(false);
        AssertAntecedent(new Antecedent, std::move(engine))
            .input_has_activation_degrees(
                {// inputs
                 {"Ambient", {0.0, 0.2, 0.4, 0.5, 0.6, 0.8, 1.0}}
                },
                {// rules
                 {"Ambient is DARK", {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
                 {"Ambient is MEDIUM", {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
                 {"Ambient is BRIGHT", {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}
                }
            );
    }

    TEST_CASE("Rule/Antecedent/ActivationDegrees/EnabledVariable", "[rule][antecedent]") {
        engine->getInputVariable("Ambient")->setEnabled(true);
        AssertAntecedent(new Antecedent, std::move(engine))
            .input_has_activation_degrees(
                {// inputs
                 {"Ambient", {0.0, 0.2, 0.4, 0.5, 0.6, 0.8, 1.0}}
                },
                {// rules
                 {"Ambient is DARK", {0.0, 0.8, 0.4, 0.0, 0.0, 0.0, 0.0}},
                 {"Ambient is MEDIUM", {0.0, 0.0, 0.6, 1.0, 0.6, 0.0, 0.0}},
                 {"Ambient is BRIGHT", {0.0, 0.0, 0.0, 0.0, 0.4, 0.8, 0.0}}
                }
            );
    }

    TEST_CASE("Rule/Antecedent/ActivationDegrees/Hedges", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .input_has_activation_degrees(
                {// inputs
                 {"Ambient", {0.0, 0.2, 0.4, 0.5, 0.6, 0.8, 1.0}}
                },
                {// rules
                 {"Ambient is very DARK", {0.0, 0.64, 0.160, 0.0, 0.000, 0.000, 0.0}},
                 {"Ambient is somewhat MEDIUM", {0.0, 0.00, 0.775, 1.0, 0.775, 0.000, 0.0}},
                 {"Ambient is seldom BRIGHT", {0.0, 0.00, 0.000, 0.0, 0.447, 0.684, 0.0}}
                }
            );
    }

    TEST_CASE("Rule/Antecedent/ActivationDegrees/MultipleHedges", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .input_has_activation_degrees(
                {// inputs
                 {"Ambient", {0.0, 0.2, 0.4, 0.5, 0.6, 0.8, 1.0}}
                },
                {// rules
                 {"Ambient is very very DARK", {0.0, 0.41, 0.026, 0.0, 0.000, 0.000, 0.0}},
                 {"Ambient is somewhat very MEDIUM", {0.0, 0.00, 0.600, 1.0, 0.600, 0.000, 0.0}},
                 {"Ambient is seldom very BRIGHT", {0.0, 0.00, 0.000, 0.0, 0.283, 0.576, 0.0}}
                }
            );
    }

    TEST_CASE("Rule/Antecedent/ActivationDegrees/SpecialHedges", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .input_has_activation_degrees(
                {// inputs
                 {"Ambient", {0.0, 0.2, 0.4, 0.5, 0.6, 0.8, 1.0}}
                },
                {// rules
                 {"Ambient is any", {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}},
                 {"Ambient is not any", {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}},
                 {"Ambient is not not any", {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}}
                }
            );
    }

    TEST_CASE("Rule/Antecedent/ActivationDegrees/TNorms", "[rule][antecedent]") {
        const Minimum minimum;
        AssertAntecedent(new Antecedent, std::move(engine))
            .input_has_activation_degrees(
                {// inputs
                 {"Ambient", {0.0, 0.2, 0.4, 0.5, 0.6, 0.8, 1.0}}
                },
                {// rules
                 {"Ambient is DARK and Ambient is MEDIUM", {0.0, 0.0, 0.4, 0.0, 0.0, 0.0, 0.0}},
                 {"Ambient is MEDIUM and Ambient is BRIGHT", {0.0, 0.0, 0.0, 0.0, 0.4, 0.0, 0.0}},
                 {"Ambient is BRIGHT and Ambient is DARK", {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}}
                },
                &minimum
            );
    }

    TEST_CASE("Rule/Antecedent/ActivationDegrees/SNorms", "[rule][antecedent]") {
        const Maximum maximum;
        AssertAntecedent(new Antecedent, std::move(engine))
            .input_has_activation_degrees(
                {// inputs
                 {"Ambient", {0.0, 0.2, 0.4, 0.5, 0.6, 0.8, 1.0}}
                },
                {// rules
                 {"Ambient is DARK or Ambient is MEDIUM", {0.0, 0.8, 0.6, 1.0, 0.6, 0.0, 0.0}},
                 {"Ambient is MEDIUM or Ambient is BRIGHT", {0.0, 0.0, 0.6, 1.0, 0.6, 0.8, 0.0}},
                 {"Ambient is BRIGHT or Ambient is DARK", {0.0, 0.8, 0.4, 0.0, 0.4, 0.8, 0.0}}
                },
                fl::null,
                &maximum
            );
    }

    TEST_CASE("Rule/Antecedent/ActivationDegrees/OutputVariables", "[rule][antecedent]") {
        const OutputVariable* power = engine->getOutputVariable("Power");
        const Term* low = power->getTerm("LOW");
        const Term* medium = power->getTerm("MEDIUM");
        const Term* high = power->getTerm("HIGH");
        AssertAntecedent(new Antecedent, std::move(engine))
            .output_has_activation_degrees(
                {// outputs
                 {"Power",
                  {
                      // fuzzy values
                      {fl::Activated(low, 0.0), fl::Activated(medium, 0.0), fl::Activated(high, 0.0)},
                      {fl::Activated(low, 0.0), fl::Activated(medium, 0.0), fl::Activated(high, 1.0)},
                      {fl::Activated(low, 0.0), fl::Activated(medium, 1.0), fl::Activated(high, 0.0)},
                      {fl::Activated(low, 0.0), fl::Activated(medium, 1.0), fl::Activated(high, 1.0)},
                      {fl::Activated(low, 1.0), fl::Activated(medium, 0.0), fl::Activated(high, 0.0)},
                      {fl::Activated(low, 1.0), fl::Activated(medium, 0.0), fl::Activated(high, 1.0)},
                      {fl::Activated(low, 1.0), fl::Activated(medium, 1.0), fl::Activated(high, 0.0)},
                      {fl::Activated(low, 1.0), fl::Activated(medium, 1.0), fl::Activated(high, 1.0)},
                  }}
                },
                {// rules
                 {"Power is LOW", {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0}},
                 {"Power is MEDIUM", {0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0}},
                 {"Power is HIGH", {0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0}}
                }
            );
    }

}}
