#include "Headers.h"
#include "Mock.h"

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

    TEST_CASE("Activation: Constructors", "[activation][constructors]") {
        SECTION("General") {
            AssertConstructor().can_copy_and_move(General(), &Activation::toString);
        }
        SECTION("First") {
            AssertConstructor().can_copy_and_move(First(), &Activation::toString);
            AssertConstructor().can_copy_and_move(First(3, 0.5), &Activation::toString);
        }
        SECTION("Last") {
            AssertConstructor().can_copy_and_move(Last(), &Activation::toString);
            AssertConstructor().can_copy_and_move(Last(3, 0.5), &Activation::toString);
        }
        SECTION("Highest") {
            AssertConstructor().can_copy_and_move(Highest(), &Activation::toString);
            AssertConstructor().can_copy_and_move(Highest(5), &Activation::toString);
        }
        SECTION("Lowest") {
            AssertConstructor().can_copy_and_move(Lowest(), &Activation::toString);
            AssertConstructor().can_copy_and_move(Lowest(5), &Activation::toString);
        }
        SECTION("Proportional") {
            AssertConstructor().can_copy_and_move(Proportional(), &Activation::toString);
        }
        SECTION("Threshold") {
            AssertConstructor().can_copy_and_move(Threshold(), &Activation::toString);
            AssertConstructor().can_copy_and_move(Threshold(">", 0.5), &Activation::toString);
        }
    }

    TEST_CASE("Activation: FLL", "[activation][fll]") {
        SECTION("General") {
            CHECK(General().toString() == "General");
        }
        SECTION("First") {
            CHECK(First().toString() == "First 1 0.000");
            CHECK(First(3, 0.5).toString() == "First 3 0.500");
        }
        SECTION("Last") {
            CHECK(Last().toString() == "Last 1 0.000");
            CHECK(Last(3, 0.5).toString() == "Last 3 0.500");
        }
        SECTION("Highest") {
            CHECK(Highest().toString() == "Highest 1");
            CHECK(Highest(5).toString() == "Highest 5");
        }
        SECTION("Lowest") {
            CHECK(Lowest().toString() == "Lowest 1");
            CHECK(Lowest(5).toString() == "Lowest 5");
        }
        SECTION("Proportional") {
            CHECK(Proportional().toString() == "Proportional");
        }
        SECTION("Threshold") {
            CHECK(Threshold().toString() == "Threshold >= 0.000");
            CHECK(Threshold("<", 1.0).toString() == "Threshold < 1.000");
            CHECK(Threshold("<=", 1.0).toString() == "Threshold <= 1.000");
            CHECK(Threshold("==", 0.5).toString() == "Threshold == 0.500");
            CHECK(Threshold("!=", 0.5).toString() == "Threshold != 0.500");
            CHECK(Threshold(">=", 1.0).toString() == "Threshold >= 1.000");
            CHECK(Threshold(">", 1.0).toString() == "Threshold > 1.000");
        }
    }

    TEST_CASE("Activation: Configurable", "[activation][configurable]") {
        SECTION("General") {
            AssertActivation(RuleBlock().activation(new General)).is_configurable();
        }
        SECTION("First") {
            AssertActivation(RuleBlock().activation(new First())).is_configurable();
            AssertActivation(RuleBlock().activation(new First(3))).is_configurable();
            AssertActivation(RuleBlock().activation(new First(3, 0.5))).is_configurable();

            AssertActivation(RuleBlock().activation(new First(1, 0.333))).is_configurable("1 0.333 4 5");
            AssertActivation(RuleBlock().activation(new First(1))).fails_to_configure("1", 2);
        }
        SECTION("Last") {
            AssertActivation(RuleBlock().activation(new Last())).is_configurable();
            AssertActivation(RuleBlock().activation(new Last(3))).is_configurable();
            AssertActivation(RuleBlock().activation(new Last(3, 0.5))).is_configurable();

            AssertActivation(RuleBlock().activation(new Last(1, 0.333))).is_configurable("1 0.333 4 5");
            AssertActivation(RuleBlock().activation(new Last(1))).fails_to_configure("1", 2);
        }
        SECTION("Highest") {
            AssertActivation(RuleBlock().activation(new Highest())).is_configurable();
            AssertActivation(RuleBlock().activation(new Highest(3))).is_configurable("3");
            AssertActivation(RuleBlock().activation(new Highest(1))).is_configurable("1 0.333 4 5");
        }
        SECTION("Lowest") {
            AssertActivation(RuleBlock().activation(new Lowest())).is_configurable();
            AssertActivation(RuleBlock().activation(new Lowest(3))).is_configurable("3");
            AssertActivation(RuleBlock().activation(new Lowest(1))).is_configurable("1 0.333 4 5");
        }
        SECTION("Proportional") {
            AssertActivation(RuleBlock().activation(new Proportional())).is_configurable();
        }
        SECTION("Threshold") {
            AssertActivation(RuleBlock().activation(new Threshold())).is_configurable();
            AssertActivation(RuleBlock().activation(new Threshold(">", 0.1))).is_configurable("> 0.100");
            AssertActivation(RuleBlock().activation(new Threshold(">", 0.1))).is_configurable("> 0.100 0 1 2");

            AssertActivation(RuleBlock().activation(new Threshold(">", 0.1))).fails_to_configure(">", 2);

            const std::vector<std::string> expected_operators = {"<", "<=", "==", "!=", ">=", ">"};
            CHECK(Threshold().availableComparisonOperators() == expected_operators);

            CHECK(Threshold().comparisonOperator((Threshold::Comparison)9) == "?");

            Threshold threshold;
            threshold.setThreshold(">", 0.1);
            CHECK(threshold.toString() == "Threshold > 0.100");

            threshold.setThreshold(Threshold::Comparison::EqualTo, 0.5);
            CHECK(threshold.toString() == "Threshold == 0.500");
        }
    }

    TEST_CASE("Activation", "[activation][operation]") {
        RuleBlock rb_all_zero
            = RuleBlock("all_zero")
                  .rules({new MockRule("A", "a", 0.0), new MockRule("B", "b", 0.0), new MockRule("C", "c", 0.0)});
        RuleBlock rb_all_one = RuleBlock("all_one").rules(
            {new MockRule("A", "a", 1.0), new MockRule("B", "b", 1.0), new MockRule("C", "c", 1.0)}
        );
        RuleBlock rb_one_to_zero
            = RuleBlock("one_to_zero")
                  .rules({new MockRule("A", "a", 1.0), new MockRule("B", "b", 0.5), new MockRule("C", "c", 0.0)});

        SECTION("General") {
            AssertActivation(rb_all_zero.activation(new General))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(new General))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", true}});
            AssertActivation(rb_one_to_zero.activation(new General))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
        }
        SECTION("First") {
            // 1 Rule, 0 threshold
            First first_one(1);
            AssertActivation(rb_all_zero.activation(first_one.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(new First))
                .triggers({{"A", "a", true}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_one_to_zero.activation(new First))
                .triggers({{"A", "a", true}, {"B", "b", false}, {"C", "c", false}});

            // 2 Rules, 0 threshold
            First first_two(2);
            AssertActivation(rb_all_zero.activation(first_two.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(first_two.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
            AssertActivation(rb_one_to_zero.activation(first_two.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});

            // 2 Rules, 0.5 threshold
            First first_two_with_threshold(2, 0.5);
            AssertActivation(rb_all_zero.activation(first_two_with_threshold.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(first_two_with_threshold.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
            AssertActivation(rb_one_to_zero.activation(first_two_with_threshold.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
        }

        SECTION("Last") {
            // 1 Rule, 0 threshold
            Last last_one(1);
            AssertActivation(rb_all_zero.activation(last_one.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(last_one.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", true}});
            AssertActivation(rb_one_to_zero.activation(last_one.clone()))
                .triggers({{"A", "a", false}, {"B", "b", true}, {"C", "c", false}});

            // 2 Rules, 0 threshold
            Last last_two(2);
            AssertActivation(rb_all_zero.activation(last_two.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(last_two.clone()))
                .triggers({{"A", "a", false}, {"B", "b", true}, {"C", "c", true}});
            AssertActivation(rb_one_to_zero.activation(last_two.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});

            // 2 Rules, 0.5 threshold
            Last last_two_with_threshold(2, 0.5);
            AssertActivation(rb_all_zero.activation(last_two_with_threshold.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(last_two_with_threshold.clone()))
                .triggers({{"A", "a", false}, {"B", "b", true}, {"C", "c", true}});
            AssertActivation(rb_one_to_zero.activation(last_two_with_threshold.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
        }

        SECTION("Highest") {
            // 1 Rule
            Highest highest_one(1);
            AssertActivation(rb_all_zero.activation(highest_one.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(highest_one.clone()))
                .triggers({{"A", "a", true}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_one_to_zero.activation(highest_one.clone()))
                .triggers({{"A", "a", true}, {"B", "b", false}, {"C", "c", false}});

            // 2 Rules
            Highest highest_two(2);
            AssertActivation(rb_all_zero.activation(highest_two.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(highest_two.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
            AssertActivation(rb_one_to_zero.activation(highest_two.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
        }

        SECTION("Lowest") {
            // 1 Rule
            Lowest lowest_one(1);
            AssertActivation(rb_all_zero.activation(lowest_one.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(lowest_one.clone()))
                .triggers({{"A", "a", true}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_one_to_zero.activation(lowest_one.clone()))
                .triggers({{"A", "a", false}, {"B", "b", true}, {"C", "c", false}});

            // 2 Rules
            Lowest lowest_two(2);
            AssertActivation(rb_all_zero.activation(lowest_two.clone()))
                .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(lowest_two.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
            AssertActivation(rb_one_to_zero.activation(lowest_two.clone()))
                .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
        }

        SECTION("Proportional") {
            Proportional proportional;
            AssertActivation(rb_all_zero.activation(proportional.clone()))
                .triggers_with_degree({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
            AssertActivation(rb_all_one.activation(proportional.clone()))
                .triggers_with_degree({{"A", "a", 1 / 3.0}, {"B", "b", 1 / 3.0}, {"C", "c", 1 / 3.0}});
            AssertActivation(rb_one_to_zero.activation(proportional.clone()))
                .triggers_with_degree({{"A", "a", 2 / 3.0}, {"B", "b", 1 / 3.0}, {"C", "c", false}});
        }

        SECTION("Threshold") {
            SECTION("Less than") {
                Threshold lt("<", 0.6);
                AssertActivation(rb_all_zero.activation(lt.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_all_one.activation(lt.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_one_to_zero.activation(lt.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", true}, {"C", "c", false}});
            }
            SECTION("Less than or equals to") {
                Threshold le("<=", 0.5);
                AssertActivation(rb_all_zero.activation(le.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_all_one.activation(le.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_one_to_zero.activation(le.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", true}, {"C", "c", false}});
            }
            SECTION("Equals to") {
                Threshold eq("==", 0.5);
                AssertActivation(rb_all_zero.activation(eq.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_all_one.activation(eq.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_one_to_zero.activation(eq.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", true}, {"C", "c", false}});
            }
            SECTION("Not equal to") {
                Threshold neq("!=", 0.5);
                AssertActivation(rb_all_zero.activation(neq.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_all_one.activation(neq.clone()))
                    .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", true}});
                AssertActivation(rb_one_to_zero.activation(neq.clone()))
                    .triggers({{"A", "a", true}, {"B", "b", false}, {"C", "c", false}});
            }
            SECTION("Greater than or Equals to") {
                Threshold ge(">=", 0.5);
                AssertActivation(rb_all_zero.activation(ge.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_all_one.activation(ge.clone()))
                    .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", true}});
                AssertActivation(rb_one_to_zero.activation(ge.clone()))
                    .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", false}});
            }
            SECTION("Greater than") {
                Threshold gt(">", 0.5);
                AssertActivation(rb_all_zero.activation(gt.clone()))
                    .triggers({{"A", "a", false}, {"B", "b", false}, {"C", "c", false}});
                AssertActivation(rb_all_one.activation(gt.clone()))
                    .triggers({{"A", "a", true}, {"B", "b", true}, {"C", "c", true}});
                AssertActivation(rb_one_to_zero.activation(gt.clone()))
                    .triggers({{"A", "a", true}, {"B", "b", false}, {"C", "c", false}});
            }
        }
    }
}}
