/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * Antecedent.cpp
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "fl/rule/Antecedent.h"

#include "fl/rule/Expression.h"

#include "fl/Engine.h"
#include "fl/variable/InputVariable.h"
#include "fl/hedge/Hedge.h"
#include "fl/hedge/Any.h"
#include "fl/term/Term.h"

#include "fl/rule/Rule.h"
#include "fl/norm/TNorm.h"
#include "fl/norm/SNorm.h"

#include "fl/term/Function.h"

#include "fl/factory/FactoryManager.h"
#include "fl/factory/HedgeFactory.h"

#include <stack>
#include <algorithm>

namespace fl {

    Antecedent::Antecedent()
    : _text(""), _expression(NULL) {
    }

    Antecedent::~Antecedent() {
        unload();
    }

    void Antecedent::setText(const std::string& text) {
        this->_text = text;
    }

    std::string Antecedent::getText() const {
        return this->_text;
    }

    Expression* Antecedent::getExpression() const {
        return this->_expression;
    }

    bool Antecedent::isLoaded() const {
        return this->_expression;
    }

    scalar Antecedent::activationDegree(const TNorm* conjunction, const SNorm* disjunction,
            const Expression* node) const {
        if (not isLoaded()) {
            throw fl::Exception("[antecedent error] antecedent <" + _text + "> is not loaded", FL_AT);
        }
        const Proposition* proposition = dynamic_cast<const Proposition*> (node);
        if (proposition) {
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

            InputVariable* inputVariable = dynamic_cast<InputVariable*> (proposition->variable);
            scalar result = proposition->term->membership(inputVariable->getInputValue());
            for (std::vector<Hedge*>::const_reverse_iterator rit = proposition->hedges.rbegin();
                    rit != proposition->hedges.rend(); ++rit) {
                result = (*rit)->hedge(result);
            }
            return result;
        }
        //if node is an operatorsk
        const Operator* fuzzyOperator = dynamic_cast<const Operator*> (node);
        if (not (fuzzyOperator->left and fuzzyOperator->right)) {
            std::ostringstream ex;
            ex << "[syntax error] left and right operands must exist";
            throw fl::Exception(ex.str(), FL_AT);
        }
        if (fuzzyOperator->name == Rule::FL_AND)
            return conjunction->compute(
                this->activationDegree(conjunction, disjunction, fuzzyOperator->left),
                this->activationDegree(conjunction, disjunction, fuzzyOperator->right));

        if (fuzzyOperator->name == Rule::FL_OR)
            return disjunction->compute(
                this->activationDegree(conjunction, disjunction, fuzzyOperator->left),
                this->activationDegree(conjunction, disjunction, fuzzyOperator->right));
        std::ostringstream ex;
        ex << "[syntax error] operator <" << fuzzyOperator->name << "> not recognized";
        throw fl::Exception(ex.str(), FL_AT);

    }

    scalar Antecedent::activationDegree(const TNorm* conjunction, const SNorm* disjunction) const {
        return this->activationDegree(conjunction, disjunction, this->_expression);
    }

    void Antecedent::unload() {
        if (_expression) {
            delete _expression;
            _expression = NULL;
        }
    }

    void Antecedent::load(fl::Rule* rule, const Engine* engine) {
        load(_text, rule, engine);
    }

    void Antecedent::load(const std::string& antecedent, fl::Rule* rule, const Engine* engine) {
        unload();
        this->_text = antecedent;
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
        std::stringstream tokenizer(postfix);
        std::string token;

        enum FSM {
            S_VARIABLE = 1, S_IS = 2, S_HEDGE = 4, S_TERM = 8, S_AND_OR = 16
        };
        int state = S_VARIABLE;
        std::stack<Expression*> expressionStack;
        Proposition* proposition = NULL;
        try {
            while (tokenizer >> token) {
                if (state bitand S_VARIABLE) {
                    if (engine->hasInputVariable(token)) {
                        proposition = new Proposition;
                        proposition->variable = engine->getInputVariable(token);
                        expressionStack.push(proposition);

                        state = S_IS;
                        continue;
                    }
                }

                if (state bitand S_IS) {
                    if (token == Rule::FL_IS) {
                        state = S_HEDGE bitor S_TERM;
                        continue;
                    }
                }

                if (state bitand S_HEDGE) {
                    Hedge* hedge = NULL;
                    if (rule->hasHedge(token)) {
                        hedge = rule->getHedge(token);
                    } else {
                        std::vector<std::string> hedges = FactoryManager::instance()->hedge()->available();
                        if (std::find(hedges.begin(), hedges.end(), token) != hedges.end()) {
                            hedge = FactoryManager::instance()->hedge()->createInstance(token);
                            rule->addHedge(hedge);
                        }
                    }
                    if (hedge) {
                        proposition->hedges.push_back(hedge);
                        if (dynamic_cast<Any*> (hedge)) {
                            state = S_VARIABLE bitor S_AND_OR;
                        } else {
                            state = S_HEDGE bitor S_TERM;
                        }
                        continue;
                    }
                }

                if (state bitand S_TERM) {
                    if (proposition->variable->hasTerm(token)) {
                        proposition->term = proposition->variable->getTerm(token);
                        state = S_VARIABLE bitor S_AND_OR;
                        continue;
                    }
                }

                if (state bitand S_AND_OR) {
                    if (token == Rule::FL_AND or token == Rule::FL_OR) {
                        if (expressionStack.size() < 2) {
                            std::ostringstream ex;
                            ex << "[syntax error] logical operator <" << token << "> expects two operands,"
                                    << "but found " << expressionStack.size();
                            throw fl::Exception(ex.str(), FL_AT);
                        }
                        Operator* fuzzyOperator = new Operator;
                        fuzzyOperator->name = token;
                        fuzzyOperator->right = expressionStack.top();
                        expressionStack.pop();
                        fuzzyOperator->left = expressionStack.top();
                        expressionStack.pop();
                        expressionStack.push(fuzzyOperator);

                        state = S_VARIABLE bitor S_AND_OR;
                        continue;
                    }
                }

                //If reached this point, there was an error
                if ((state bitand S_VARIABLE) or (state bitand S_AND_OR)) {
                    std::ostringstream ex;
                    ex << "[syntax error] expected input variable or logical operator, but found <" << token << ">";
                    throw fl::Exception(ex.str(), FL_AT);
                }
                if (state bitand S_IS) {
                    std::ostringstream ex;
                    ex << "[syntax error] expected keyword <" << Rule::FL_IS << ">, but found <" << token << ">";
                    throw fl::Exception(ex.str(), FL_AT);
                }
                if ((state bitand S_HEDGE) or (state bitand S_TERM)) {
                    std::ostringstream ex;
                    ex << "[syntax error] expected hedge or term, but found <" << token << ">";
                    throw fl::Exception(ex.str(), FL_AT);
                }
                std::ostringstream ex;
                ex << "[syntax error] unexpected token <" << token << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }

            if (expressionStack.size() != 1) {
                std::ostringstream ex;
                ex << "[syntax error] stack expected to contain the root, but contains "
                        << expressionStack.size() << " nodes";
                throw fl::Exception(ex.str(), FL_AT);
            }
        } catch (std::exception& ex) {
            for (std::size_t i = 0; i < expressionStack.size(); ++i) {
                delete expressionStack.top();
                expressionStack.pop();
            }
        }
        this->_expression = expressionStack.top();
    }

    std::string Antecedent::toString() const {
        return toInfix(this->_expression);
    }

    std::string Antecedent::toPrefix(const Expression* node) const {
        if (not isLoaded()) {
            throw fl::Exception("[antecedent error] antecedent <" + _text + "> is not loaded", FL_AT);
        }
        if (not node) node = this->_expression;

        if (dynamic_cast<const Proposition*> (node)) {
            return node->toString();
        }
        const Operator* fuzzyOperator =
                dynamic_cast<const Operator*> (node);
        std::stringstream ss;
        ss << fuzzyOperator->toString() << " "
                << toPrefix(fuzzyOperator->left) << " "
                << toPrefix(fuzzyOperator->right) << " ";
        return ss.str();
    }

    std::string Antecedent::toInfix(const Expression* node) const {
        if (not isLoaded()) {
            throw fl::Exception("[antecedent error] antecedent <" + _text + "> is not loaded", FL_AT);
        }
        if (not node) node = this->_expression;
        if (dynamic_cast<const Proposition*> (node)) {
            return node->toString();
        }
        const Operator* fuzzyOperator =
                dynamic_cast<const Operator*> (node);
        std::stringstream ss;
        ss << toInfix(fuzzyOperator->left) << " "
                << fuzzyOperator->toString() << " "
                << toInfix(fuzzyOperator->right) << " ";
        return ss.str();
    }

    std::string Antecedent::toPostfix(const Expression* node) const {
        if (not isLoaded()) {
            throw fl::Exception("[antecedent error] antecedent <" + _text + "> is not loaded", FL_AT);
        }
        if (not node) node = this->_expression;
        if (dynamic_cast<const Proposition*> (node)) {
            return node->toString();
        }
        const Operator* fuzzyOperator =
                dynamic_cast<const Operator*> (node);
        std::stringstream ss;
        ss << toPostfix(fuzzyOperator->left) << " "
                << toPostfix(fuzzyOperator->right) << " "
                << fuzzyOperator->toString() << " ";
        return ss.str();
    }


}
