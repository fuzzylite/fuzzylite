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
#include "test/Mock.h"

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

    TEST_CASE("Rule/Antecedent/Load", "[rule][antecedent]") {
        Antecedent antecedent;
        CHECK(not antecedent.isLoaded());

        antecedent.setExpression(new Proposition());
        CHECK(antecedent.isLoaded());

        antecedent.unload();
        CHECK(not antecedent.isLoaded());
        CHECK(not antecedent.getExpression());
    }

    TEST_CASE("Rule/Antecedent/Load/input variable", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Ambient is DARK")
            .toInfix()
            .can_load_antecedent("Ambient is very DARK")
            .toInfix()
            .can_load_antecedent("Ambient is any")
            .toInfix();
    }

    TEST_CASE("Rule/Antecedent/Load/output variable", "[rule][antecedent]") {
        AssertAntecedent(new Antecedent, std::move(engine))
            .can_load_antecedent("Power is HIGH")
            .toInfix()
            .can_load_antecedent("Power is very HIGH")
            .toInfix()
            .can_load_antecedent("Power is any")
            .toInfix();
    }

    TEST_CASE("Rule/Antecedent/Load/input variable/connectors", "[rule][antecedent]") {
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

    TEST_CASE("Rule/Antecedent/Load/output variable/connectors", "[rule][antecedent]") {
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

    TEST_CASE("Rule/Antecedent/Load/Fails", "[rule][antecedent]") {
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

    TEST_CASE("Rule/Consequent/Basic", "[rule][consequent]") {
        Consequent consequent;
        CHECK(not consequent.isLoaded());

        consequent.conclusions().push_back(new Proposition);
        CHECK(consequent.isLoaded());

        consequent.unload();
        CHECK(not consequent.isLoaded());
        CHECK(consequent.conclusions().empty());
    }

    TEST_CASE("Rule/Consequent/Load", "[rule][consequent]") {
        AssertConsequent(new Consequent, std::move(engine))
            .can_load_consequent("Power is HIGH")
            .can_load_consequent("Power is MEDIUM")
            .can_load_consequent("Power is LOW");
    }

    TEST_CASE("Rule/Consequent/Load/Multiple", "[rule][consequent]") {
        AssertConsequent(new Consequent, std::move(engine))
            .can_load_consequent("Power is HIGH and Power is HIGH")
            .can_load_consequent("Power is very HIGH and Power is very HIGH")
            .can_load_consequent("Power is any LOW and Power is not any HIGH");
    }

    TEST_CASE("Rule/Consequent/CannotLoad", "[rule][consequent]") {
        AssertConsequent(new Consequent, std::move(engine))
            .cannot_load_consequent("", "[syntax error] consequent is empty")
            .cannot_load_consequent("Power HIGH", "[syntax error] consequent expected keyword <is>, but found <HIGH>")
            .cannot_load_consequent("Power is ALL", "[syntax error] consequent expected hedge or term, but found <ALL>")
            .cannot_load_consequent(
                "Power is very ALL", "[syntax error] consequent expected hedge or term, but found <ALL>"
            )
            .cannot_load_consequent(
                "Power is very HIGH or Power is very HIGH",
                "[syntax error] consequent expected operator <and> or keyword <with>, but found <or>"
            )
            .cannot_load_consequent("Power", "[syntax error] consequent expected keyword <is> after <Power>")
            .cannot_load_consequent("Power is", "[syntax error] consequent expected hedge or term after <is>")
            .cannot_load_consequent("Power is very", "[syntax error] consequent expected hedge or term after <very>")
            .cannot_load_consequent(
                "Power is very LOW and", "[syntax error] consequent expected output variable after <and>"
            );
    }

    TEST_CASE("Rule/Consequent/Modify", "[rule][consequent]") {
        auto* power = engine->getOutputVariable("Power");
        auto* low = power->getTerm("LOW");
        auto* high = power->getTerm("HIGH");

        auto minimum = fl::Minimum();
        auto product = fl::AlgebraicProduct();

        AssertConsequent(new Consequent, std::move(engine))
            .modify_consequent("Power is LOW", 0.5, {{"Power", {fl::Activated(low, 0.5)}}})
            .modify_consequent("Power is LOW", 0.5, {{"Power", {fl::Activated(low, 0.5)}}}, fl::null)
            .modify_consequent("Power is very LOW", 0.5, {{"Power", {fl::Activated(low, 0.25)}}}, &minimum)
            .modify_consequent(
                "Power is LOW and Power is HIGH",
                0.25,
                {{"Power", {fl::Activated(low, 0.25), fl::Activated(high, 0.25)}}},
                &product
            )
            .modify_consequent(
                "Power is LOW and Power is very HIGH",
                0.5,
                {{"Power", {fl::Activated(low, 0.5), fl::Activated(high, 0.25)}}}
            );
    }

    TEST_CASE("Rule/Consequent/Modify/DisabledVariable", "[rule][consequent]") {
        engine->getOutputVariable("Power")->setEnabled(false);
        AssertConsequent(new Consequent, std::move(engine))
            .modify_consequent("Power is LOW and Power is very HIGH", 0.5, {{"Power", {}}});
    }

    TEST_CASE("Rule/Consequent/Modify/Cannot", "[rule][consequent]") {
        Consequent consequent;
        Minimum minimum;
        CHECK_THROWS_AS(consequent.modify(fl::nan, &minimum), fl::Exception);
        CHECK_THROWS_WITH(
            consequent.modify(fl::nan, &minimum),
            Catch::Matchers::StartsWith("[consequent error] consequent <> is not loaded")
        );
    }

    Rule testRule() {
        Rule rule{
            "if Ambient is DARK then Power is LOW",
            0.5,
        };
        rule.setEnabled(true);
        rule.setActivationDegree(0.75);
        rule.setTriggered(true);
        rule.getAntecedent()->setText("Ambient is DARK");
        rule.getAntecedent()->setExpression(new Proposition);  // to load it
        rule.getConsequent()->setText("Power is LOW");
        rule.getConsequent()->conclusions().push_back(new Proposition);  // to load it;
        return rule;
    }

    TEST_CASE("Rule/Constructor", "[rule][constructor]") {
        AssertRule(std::make_unique<Rule>(), engine.get())
            .has_text("")
            .is_enabled()
            .is_loaded(false)
            .is_triggered(false)
            .has_weight(1.0)
            .has_activation_degree(0.0)
            .has_antecedent("")
            .has_consequent("");

        AssertRule(std::make_unique<Rule>("if Ambient is DARK then Power is LOW", 0.5), engine.get())
            .has_text("if Ambient is DARK then Power is LOW")
            .is_enabled()
            .is_loaded(false)
            .has_weight(0.5)
            .has_activation_degree(0.0)
            .is_triggered(false)
            .has_antecedent("")
            .has_consequent("");
    }

    TEST_CASE("Rule/Constructor/Copy", "[rule][constructor]") {
        AssertRule(std::make_unique<Rule>(testRule()), engine.get())
            .is_enabled()
            .is_loaded()
            .is_triggered()
            .has_text("if Ambient is DARK then Power is LOW")
            .has_weight(0.5)
            .has_activation_degree(0.75)
            .has_antecedent("Ambient is DARK")
            .has_consequent("Power is LOW");
    }

    TEST_CASE("Rule/Constructor/CopyAssignment", "[rule][constructor]") {
        // TODO: Deep copy antecedent and consequent
        Rule copy("if Power is LOW then Ambient is DARK", 0.0);
        Rule test = testRule();
        copy = test;
        AssertRule(std::make_unique<Rule>(copy), engine.get())
            .is_enabled()
            .is_loaded(false)
            .is_triggered(false)
            .has_text("if Ambient is DARK then Power is LOW")
            .has_weight(0.5)
            .has_activation_degree(0.75)
            .has_antecedent("")
            .has_consequent("");
    }

    TEST_CASE("Rule/Constructor/Clone", "[rule][constructor]") {
        // TODO: Deep copy antecedent and consequent
        std::unique_ptr<Rule> clone(testRule().clone());
        AssertRule(std::make_unique<Rule>(*clone.get()), engine.get())
            .is_enabled()
            .is_loaded(false)
            .is_triggered(false)
            .has_text("if Ambient is DARK then Power is LOW")
            .has_weight(0.5)
            .has_activation_degree(0.75)
            .has_antecedent("")
            .has_consequent("");
    }

    TEST_CASE("Rule/ParseAndLoad", "[rule]") {
        AssertRule(std::make_unique<Rule>(), engine.get())
            .can_parse_rule("if Ambient is DARK then Power is LOW")
            .can_parse_rule("if Ambient is DARK then Power is LOW with 0.500")
            .can_parse_rule("if Ambient is DARK and Ambient is BRIGHT then Power is LOW and Power is HIGH with 0.500")
            .can_parse_rule("if Ambient is DARK or Ambient is BRIGHT then Power is LOW and Power is HIGH with 0.500");
    }

    TEST_CASE("Rule/Parse/Error", "[rule]") {
        AssertRule(std::make_unique<Rule>(), engine.get())
            .cannot_parse_rule("", "[syntax error] empty rule")
            .cannot_parse_rule("if", "[syntax error] keyword <then> not found in rule: if")
            .cannot_parse_rule("then", "[syntax error] expected keyword <if>, but found <then> in rule: then")
            .cannot_parse_rule("if then", "[syntax error] antecedent is empty")
            .cannot_parse_rule("if Ambient is DARK then", "[syntax error] consequent is empty")
            .cannot_parse_rule(
                "if Ambient is DARK then Power is LOW with",
                "[syntax error] expected a numeric value as the weight of the rule: "
                "if Ambient is DARK then Power is LOW with"
            )
            .cannot_parse_rule(
                "if Ambient is DARK then Power is LOW with 0.5 extra",
                "[syntax error] unexpected token <extra> at the end of rule"
            );
    }

    TEST_CASE("Rule/Deactivate", "[rule]") {
        Rule rule;
        rule.setActivationDegree(fl::nan);
        rule.setTriggered(true);
        CHECK_THAT(rule.getActivationDegree(), Approximates(fl::nan));
        CHECK(rule.isTriggered());

        rule.deactivate();

        CHECK_THAT(rule.getActivationDegree(), Approximates(0));
        CHECK(not rule.isTriggered());
    }

    TEST_CASE("Rule/Unload", "[rule]") {
        Minimum implication;
        Rule rule("if Ambient is DARK then Power is LOW");
        rule.load(engine.get());

        rule.setActivationDegree(0.5);
        rule.trigger(&implication);
        CHECK(rule.isLoaded());
        CHECK(rule.isTriggered());
        CHECK_THAT(rule.getActivationDegree(), Approximates(0.5));

        rule.unload();
        CHECK(not rule.isLoaded());
        CHECK(not rule.isTriggered());
        CHECK_THAT(rule.getActivationDegree(), Approximates(0));
    }

    TEST_CASE("Rule/Loaded", "[rule]") {
        Rule rule("if Ambient is DARK then Power is LOW");
        rule.load(engine.get());

        SECTION("Antecedent and Consequent: Loaded") {
            CHECK(rule.getAntecedent()->isLoaded());
            CHECK(rule.getConsequent()->isLoaded());
            CHECK(rule.isLoaded());
        }
        SECTION("Antecedent and not Consequent: Not Loaded") {
            rule.getConsequent()->unload();
            CHECK(rule.getAntecedent()->isLoaded());
            CHECK(not rule.getConsequent()->isLoaded());
            CHECK(not rule.isLoaded());
        }
        SECTION("not Antecedent and Consequent: Not Loaded") {
            rule.getAntecedent()->unload();
            CHECK(not rule.getAntecedent()->isLoaded());
            CHECK(rule.getConsequent()->isLoaded());
            CHECK(not rule.isLoaded());
        }
        SECTION("not Antecedent and not Consequent: Not Loaded") {
            rule.getAntecedent()->unload();
            rule.getConsequent()->unload();
            CHECK(not rule.getAntecedent()->isLoaded());
            CHECK(not rule.getConsequent()->isLoaded());
            CHECK(not rule.isLoaded());
        }
    }

    TEST_CASE("Rule/ActivateWith/Fails", "[rule]") {
        Rule rule("if true then false");
        CHECK(not rule.isLoaded());

        CHECK_THROWS_AS(rule.activateWith(fl::null, fl::null), fl::Exception);
        CHECK_THROWS_WITH(
            rule.activateWith(fl::null, fl::null),
            Catch::Matchers::StartsWith("[rule error] the following rule is not loaded: if true then false")
        );
    }

    TEST_CASE("Rule/ActivateWith", "[rule]") {
        std::unique_ptr<Rule> rule(Rule::parse("if Ambient is DARK then Power is HIGH", engine.get()));
        CHECK(rule->isLoaded());

        rule->setAntecedent(new MockAntecedent("Ambient is MOCK", 0.5));

        std::vector<fl::scalar> weights = {0.0, 0.1, 0.25, 0.5, 0.75, 0.9, 1.0, -fl::inf, fl::inf};
        std::vector<fl::scalar> expected = {0.0, 0.05, 0.125, 0.25, 0.375, 0.45, 0.5, -fl::inf, fl::inf};
        CHECK(weights.size() == expected.size());
        CHECK(weights.size() > 0);

        for (std::size_t index = 0; index < expected.size(); ++index) {
            CAPTURE(index);
            rule->setWeight(weights.at(index));
            const auto obtained = rule->activateWith(fl::null, fl::null);
            CHECK_THAT(obtained, Approximates(expected.at(index)));
        }
    }

    TEST_CASE("Rule/Trigger/Fails", "[rule]") {
        Minimum minimum;
        Rule rule("if true then false");
        CHECK(not rule.isLoaded());

        CHECK_THROWS_AS(rule.trigger(&minimum), fl::Exception);
        CHECK_THROWS_WITH(
            rule.trigger(&minimum),
            Catch::Matchers::StartsWith("[rule error] the following rule is not loaded: if true then false")
        );
    }

    TEST_CASE("Rule/Trigger", "[rule]") {
        std::unique_ptr<Rule> rule(Rule::parse("if Ambient is DARK then Power is HIGH", engine.get()));
        CHECK(rule->isLoaded());
        CHECK(not rule->isTriggered());
        rule->setConsequent(new MockConsequent("Power is HIGH"));

        SECTION("not enabled and activation 0") {
            rule->setEnabled(false);
            rule->setActivationDegree(0.0);
            rule->trigger(fl::null);
            CHECK(not rule->isTriggered());
        }
        SECTION("enabled and activation 0") {
            rule->setEnabled(true);
            rule->setActivationDegree(0.0);
            rule->trigger(fl::null);
            CHECK(not rule->isTriggered());
        }
        SECTION("not enabled and activation 1") {
            rule->setEnabled(false);
            rule->setActivationDegree(1.0);
            rule->trigger(fl::null);
            CHECK(not rule->isTriggered());
        }
        SECTION("not enabled and activation 1") {
            rule->setEnabled(true);
            rule->setActivationDegree(1.0);
            rule->trigger(fl::null);
            CHECK(rule->isTriggered());
        }
    }

}}
