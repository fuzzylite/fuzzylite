#ifndef FL_MOCKRULE_H
#define FL_MOCKRULE_H

#include "Headers.h"

namespace fuzzylite { namespace test {
    struct MockAntecedent : Antecedent {
        scalar _activation_degree;
        bool _loaded;

        explicit MockAntecedent(const std::string& text = "", scalar activation_degree = 0.0, bool loaded = true) :
            _activation_degree(activation_degree),
            _loaded(loaded) {
            Antecedent::setText(text);
        }

        bool isLoaded() const override {
            return _loaded;
        }

        scalar
        activationDegree(const TNorm* conjunction, const SNorm* disjunction, const Expression* node) const override {
            (void)conjunction;
            (void)disjunction;
            (void)node;
            return _activation_degree;
        }
    };

    struct MockConsequent : Consequent {
        bool _loaded;

        explicit MockConsequent(const std::string& text = "", bool loaded = true) : _loaded(loaded) {
            Consequent::setText(text);
        }

        bool isLoaded() override {
            return _loaded;
        }
    };

    struct MockRule : Rule {
        std::string _variable;
        std::string _term;
        scalar _activation_degree;
        scalar _weight;
        bool _loaded;

        explicit MockRule(
            const std::string& variable,
            const std::string& term,
            scalar activation_degree = 0.0,
            scalar weight = 1.0,
            bool loaded = true
        ) :
            _variable(variable),
            _term(term),
            _activation_degree(activation_degree),
            _weight(weight),
            _loaded(loaded) {
            Rule::setAntecedent(new MockAntecedent(variable + " is " + term, activation_degree, loaded));
            Rule::setConsequent(new MockConsequent(variable + " is " + term, loaded));
            Rule::setText(Op::join(
                std::vector<std::string>{
                    "if",
                    Rule::getAntecedent()->getText(),  //
                    "then",
                    Rule::getConsequent()->getText(),
                    "with",
                    Op::str(weight)
                },
                " "
            ));
        }

        MockRule* clone() const override {
            MockRule* result = new MockRule(_variable, _term, getActivationDegree(), _weight, _loaded);
            result->setAntecedent(new MockAntecedent(getAntecedent()->getText(), getActivationDegree(), isLoaded()));
            result->setConsequent(new MockConsequent(getConsequent()->getText(), isLoaded()));
            return result;
        }

        MockRule& self() {
            return *this;
        }

        MockRule& variable(const std::string& name) {
            _variable = name;
            return self();
        }

        MockRule& term(const std::string& name) {
            _term = name;
            return self();
        }

        MockRule& activate(scalar degree) {
            setActivationDegree(degree);
            return self();
        }

        MockRule& weight(scalar importance) {
            _weight = importance;
            return self();
        }

        MockRule& loaded(bool loaded) {
            _loaded = loaded;
            return self();
        }

        MockRule* pointer() {
            return this;
        }

        // Overrides

        scalar getActivationDegree() const override {
            return _activation_degree;
        }

        scalar getRuleActivationDegree() const {
            return Rule::getActivationDegree();
        }

        scalar activateWith(const TNorm* conjunction, const SNorm* disjunction) override {
            (void)conjunction;
            (void)disjunction;
            return getActivationDegree();
        }

        scalar getWeight() const override {
            return _weight;
        }

        bool isLoaded() const override {
            return _loaded;
        }
    };
}}

#endif
