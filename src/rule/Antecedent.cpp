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

#include "fl/rule/Antecedent.h"

#include "fl/Engine.h"
#include "fl/factory/HedgeFactory.h"
#include "fl/factory/FactoryManager.h"
#include "fl/hedge/Any.h"
#include "fl/rule/Expression.h"
#include "fl/rule/Rule.h"
#include "fl/term/Aggregated.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#include <stack>

namespace fl {

    Antecedent::Antecedent()
    : _text(""), _expression(fl::null) { }

    Antecedent::~Antecedent() {
        _expression.reset(fl::null);
    }

    void Antecedent::setText(const std::string& text) {
        this->_text = text;
    }

    std::string Antecedent::getText() const {
        return this->_text;
    }

    Expression* Antecedent::getExpression() const {
        return this->_expression.get();
    }

    void Antecedent::setExpression(Expression* expression) {
        this->_expression.reset(expression);
    }

    bool Antecedent::isLoaded() const {
        return _expression.get() != fl::null;
    }

    scalar Antecedent::activationDegree(const TNorm* conjunction, const SNorm* disjunction) const {
        return this->activationDegree(conjunction, disjunction, _expression.get());
    }

    scalar Antecedent::activationDegree(const TNorm* conjunction, const SNorm* disjunction,
            const Expression* node) const {
        if (not isLoaded()) {
            throw Exception("[antecedent error] antecedent <" + getText() + "> is not loaded", FL_AT);
        }
        const Expression::Type expression = node->type();
        if (expression == Expression::Proposition) {
            const Proposition* proposition = static_cast<const Proposition*> (node);
            if (not proposition->variable->isEnabled()) {
                return 0.0;
            }

            if (not proposition->hedges.empty()) {
                //if last hedge is "Any", apply hedges in reverse order and return degree
                std::vector<Hedge*>::const_reverse_iterator rit = proposition->hedges.rbegin();
                if (dynamic_cast<Any*> (*rit)) {
                    scalar result = (*rit)->hedge(fl::nan);
                    while (++rit != proposition->hedges.rend()) {
                        result = (*rit)->hedge(result);
                    }
                    return result;
                }
            }
            scalar result = fl::nan;
            Variable::Type variableType = proposition->variable->type();
            if (variableType == Variable::Input) {
                result = proposition->term->membership(proposition->variable->getValue());
            } else if (variableType == Variable::Output) {
                result = static_cast<OutputVariable*> (proposition->variable)
                        ->fuzzyOutput()->activationDegree(proposition->term);
            }

            if (not proposition->hedges.empty()) {
                for (std::vector<Hedge*>::const_reverse_iterator rit = proposition->hedges.rbegin();
                        rit != proposition->hedges.rend(); ++rit) {
                    result = (*rit)->hedge(result);
                }
            }
            return result;
        }
        //if node is an operator
        if (expression == Expression::Operator) {
            const Operator* fuzzyOperator = static_cast<const Operator*> (node);
            if (not (fuzzyOperator->left and fuzzyOperator->right)) {
                std::ostringstream ex;
                ex << "[syntax error] left and right operands must exist";
                throw Exception(ex.str(), FL_AT);
            }
            if (fuzzyOperator->name == Rule::andKeyword()) {
                if (not conjunction) throw Exception("[conjunction error] "
                        "the following rule requires a conjunction operator:\n" + _text, FL_AT);
                return conjunction->compute(
                        this->activationDegree(conjunction, disjunction, fuzzyOperator->left),
                        this->activationDegree(conjunction, disjunction, fuzzyOperator->right));
            }

            if (fuzzyOperator->name == Rule::orKeyword()) {
                if (not disjunction) throw Exception("[disjunction error] "
                        "the following rule requires a disjunction operator:\n" + _text, FL_AT);
                return disjunction->compute(
                        this->activationDegree(conjunction, disjunction, fuzzyOperator->left),
                        this->activationDegree(conjunction, disjunction, fuzzyOperator->right));
            }
            std::ostringstream ex;
            ex << "[syntax error] operator <" << fuzzyOperator->name << "> not recognized";
            throw Exception(ex.str(), FL_AT);

        } else {
            std::ostringstream ss;
            ss << "[antecedent error] expected a Proposition or Operator, but found <";
            if (node) ss << node->toString();
            ss << ">";
            throw Exception(ss.str(), FL_AT);
        }
    }


    Complexity Antecedent::complexity(const TNorm* conjunction, const SNorm* disjunction) const {
        return complexity(conjunction, disjunction, _expression.get());
    }

    Complexity Antecedent::complexity(const TNorm* conjunction, const SNorm* disjunction,
            const Expression* node) const {
        if (not isLoaded()) {
            return Complexity();
        }

        Complexity result;
        const Expression::Type expression = node->type();
        if (expression == Expression::Proposition) {
            const Proposition* proposition = static_cast<const Proposition*> (node);
            if (not proposition->variable->isEnabled()) {
                return result;
            }

            if (not proposition->hedges.empty()) {
                //if last hedge is "Any", apply hedges in reverse order and return degree
                std::vector<Hedge*>::const_reverse_iterator rit = proposition->hedges.rbegin();
                if (dynamic_cast<Any*> (*rit)) {
                    result += (*rit)->complexity();
                    while (++rit != proposition->hedges.rend()) {
                        result = (*rit)->complexity();
                    }
                    return result;
                }
            }
            Variable::Type variableType = proposition->variable->type();
            if (variableType == Variable::Input) {
                result += proposition->term->complexity();
            } else if (variableType == Variable::Output) {
                OutputVariable* outputVariable = static_cast<OutputVariable*> (proposition->variable);
                result += outputVariable->fuzzyOutput()->complexityOfActivationDegree();
            }

            if (not proposition->hedges.empty()) {
                for (std::vector<Hedge*>::const_reverse_iterator rit = proposition->hedges.rbegin();
                        rit != proposition->hedges.rend(); ++rit) {
                    result += (*rit)->complexity();
                }
            }
            return result;
        }
        //if node is an operator
        if (expression == Expression::Operator) {
            const Operator* fuzzyOperator = static_cast<const Operator*> (node);
            if (not (fuzzyOperator->left and fuzzyOperator->right)) {
                std::ostringstream ex;
                ex << "[syntax error] left and right operands must exist";
                throw Exception(ex.str(), FL_AT);
            }
            if (fuzzyOperator->name == Rule::andKeyword()) {
                if (conjunction) {
                    result += conjunction->complexity();
                }
                result += complexity(conjunction, disjunction, fuzzyOperator->left)
                        + complexity(conjunction, disjunction, fuzzyOperator->right);
                return result;
            }

            if (fuzzyOperator->name == Rule::orKeyword()) {
                if (disjunction) {
                    result += disjunction->complexity();
                }
                result += complexity(conjunction, disjunction, fuzzyOperator->left)
                        + complexity(conjunction, disjunction, fuzzyOperator->right);
                return result;
            }
        }
        return Complexity();
    }

    void Antecedent::unload() {
        _expression.reset(fl::null);
    }

    void Antecedent::load(const Engine* engine) {
        load(getText(), engine);
    }

    void Antecedent::load(const std::string& antecedent, const Engine* engine) {
        FL_DBG("Antecedent: " << antecedent);
        unload();
        setText(antecedent);
        if (Op::trim(antecedent).empty()) {
            throw Exception("[syntax error] antecedent is empty", FL_AT);
        }
        /*
         Builds an proposition tree from the antecedent of a fuzzy rule.
         The rules are:
         1) After a variable comes 'is',
         2) After 'is' comes a hedge or a term
         3) After a hedge comes a hedge or a term
         4) After a term comes a variable or an operator
         */

        Function function;

        std::string postfix = function.toPostfix(antecedent);
        FL_DBG("Postfix: " << postfix);
        std::stringstream tokenizer(postfix);
        std::string token;

        enum FSM {
            S_VARIABLE = 1, S_IS = 2, S_HEDGE = 4, S_TERM = 8, S_AND_OR = 16
        };
        int state = S_VARIABLE;
        std::stack<Expression*> expressionStack;
        Proposition* proposition = fl::null;
        try {
            while (tokenizer >> token) {
                if (state bitand S_VARIABLE) {
                    Variable* variable = fl::null;
                    if (engine->hasInputVariable(token))
                        variable = engine->getInputVariable(token);
                    else if (engine->hasOutputVariable(token))
                        variable = engine->getOutputVariable(token);
                    if (variable) {
                        proposition = new Proposition;
                        proposition->variable = variable;
                        expressionStack.push(proposition);

                        state = S_IS;
                        FL_DBG("Token <" << token << "> is variable");
                        continue;
                    }
                }

                if (state bitand S_IS) {
                    if (token == Rule::isKeyword()) {
                        state = S_HEDGE bitor S_TERM;
                        FL_DBG("Token <" << token << "> is keyword");
                        continue;
                    }
                }

                if (state bitand S_HEDGE) {
                    HedgeFactory* factory = FactoryManager::instance()->hedge();
                    if (factory->hasConstructor(token)) {
                        Hedge* hedge = factory->constructObject(token);
                        proposition->hedges.push_back(hedge);
                        if (dynamic_cast<Any*> (hedge)) {
                            state = S_VARIABLE bitor S_AND_OR;
                        } else {
                            state = S_HEDGE bitor S_TERM;
                        }
                        FL_DBG("Token <" << token << "> is hedge");
                        continue;
                    }
                }

                if (state bitand S_TERM) {
                    if (proposition->variable->hasTerm(token)) {
                        proposition->term = proposition->variable->getTerm(token);
                        state = S_VARIABLE bitor S_AND_OR;
                        FL_DBG("Token <" << token << "> is term");
                        continue;
                    }
                }

                if (state bitand S_AND_OR) {
                    if (token == Rule::andKeyword() or token == Rule::orKeyword()) {
                        if (expressionStack.size() < 2) {
                            std::ostringstream ex;
                            ex << "[syntax error] logical operator <" << token << "> expects two operands,"
                                    << "but found <" << expressionStack.size() << "> in antecedent";
                            throw Exception(ex.str(), FL_AT);
                        }
                        Operator* fuzzyOperator = new Operator;
                        fuzzyOperator->name = token;
                        fuzzyOperator->right = expressionStack.top();
                        expressionStack.pop();
                        fuzzyOperator->left = expressionStack.top();
                        expressionStack.pop();
                        expressionStack.push(fuzzyOperator);

                        state = S_VARIABLE bitor S_AND_OR;
                        FL_DBG("Subtree: " << fuzzyOperator->toString() <<
                                "(" << fuzzyOperator->left->toString() << ") " <<
                                "(" << fuzzyOperator->right->toString() << ")");
                        continue;
                    }
                }

                //If reached this point, there was an error
                if ((state bitand S_VARIABLE) or (state bitand S_AND_OR)) {
                    std::ostringstream ex;
                    ex << "[syntax error] antecedent expected variable or logical operator, but found <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
                if (state bitand S_IS) {
                    std::ostringstream ex;
                    ex << "[syntax error] antecedent expected keyword <" << Rule::isKeyword() << ">, but found <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
                if ((state bitand S_HEDGE) or (state bitand S_TERM)) {
                    std::ostringstream ex;
                    ex << "[syntax error] antecedent expected hedge or term, but found <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
                std::ostringstream ex;
                ex << "[syntax error] unexpected token <" << token << "> in antecedent";
                throw Exception(ex.str(), FL_AT);
            }

            if (not ((state bitand S_VARIABLE) or (state bitand S_AND_OR))) { //only acceptable final state
                if (state bitand S_IS) {
                    std::ostringstream ex;
                    ex << "[syntax error] antecedent expected keyword <" << Rule::isKeyword() << "> after <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
                if ((state bitand S_HEDGE) or (state bitand S_TERM)) {
                    std::ostringstream ex;
                    ex << "[syntax error] antecedent expected hedge or term after <" << token << ">";
                    throw Exception(ex.str(), FL_AT);
                }
            }

            if (expressionStack.size() != 1) {
                std::vector<std::string> errors;
                while (expressionStack.size() > 1) {
                    Expression* expression = expressionStack.top();
                    expressionStack.pop();
                    errors.push_back(expression->toString());
                    delete expression;
                }
                std::ostringstream ex;
                ex << "[syntax error] unable to parse the following expressions in antecedent <"
                        << Op::join(errors, " ") << ">";
                throw Exception(ex.str(), FL_AT);
            }
        } catch (...) {
            for (std::size_t i = 0; i < expressionStack.size(); ++i) {
                delete expressionStack.top();
                expressionStack.pop();
            }
            throw;
        }
        setExpression(expressionStack.top());
    }

    std::string Antecedent::toString() const {
        return toInfix(getExpression());
    }

    std::string Antecedent::toPrefix(const Expression* node) const {
        if (not isLoaded()) {
            throw Exception("[antecedent error] antecedent <" + _text + "> is not loaded", FL_AT);
        }
        if (not node) node = getExpression();

        if (dynamic_cast<const Proposition*> (node)) {
            return node->toString();
        }
        std::stringstream ss;
        if (const Operator * fuzzyOperator = dynamic_cast<const Operator*> (node)) {
            ss << fuzzyOperator->toString() << " "
                    << toPrefix(fuzzyOperator->left) << " "
                    << toPrefix(fuzzyOperator->right) << " ";
        } else {
            ss << "[antecedent error] unknown class of Expression <" << (node ? node->toString() : "null") << ">";
        }
        return ss.str();
    }

    std::string Antecedent::toInfix(const Expression* node) const {
        if (not isLoaded()) {
            throw Exception("[antecedent error] antecedent <" + _text + "> is not loaded", FL_AT);
        }
        if (not node) node = getExpression();
        if (dynamic_cast<const Proposition*> (node)) {
            return node->toString();
        }
        std::stringstream ss;
        if (const Operator * fuzzyOperator = dynamic_cast<const Operator*> (node)) {
            ss << toInfix(fuzzyOperator->left) << " "
                    << fuzzyOperator->toString() << " "
                    << toInfix(fuzzyOperator->right) << " ";
        } else {
            ss << "[antecedent error] unknown class of Expression <" << (node ? node->toString() : "null") << ">";
        }
        return ss.str();
    }

    std::string Antecedent::toPostfix(const Expression* node) const {
        if (not isLoaded()) {
            throw Exception("[antecedent error] antecedent <" + _text + "> is not loaded", FL_AT);
        }
        if (not node) node = getExpression();
        if (dynamic_cast<const Proposition*> (node)) {
            return node->toString();
        }
        std::stringstream ss;
        if (const Operator * fuzzyOperator = dynamic_cast<const Operator*> (node)) {
            ss << toPostfix(fuzzyOperator->left) << " "
                    << toPostfix(fuzzyOperator->right) << " "
                    << fuzzyOperator->toString() << " ";
        } else {
            ss << "[antecedent error] unknown class of Expression <" << (node ? node->toString() : "null") << ">";
        }
        return ss.str();
    }


}
