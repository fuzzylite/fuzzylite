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
    : _root(NULL) { }

    Antecedent::~Antecedent() {
        if (_root) delete _root;
    }

    Expression* Antecedent::getRoot() const {
        return this->_root;
    }

    scalar Antecedent::activationDegree(const TNorm* conjunction, const SNorm* disjunction,
            const Expression* node) const {
        if (not node->isOperator) { //then it is a Proposition
            const Proposition* proposition =
                    dynamic_cast<const Proposition*> (node);
            if (not proposition->variable->isEnabled()){
                return 0.0;
            }
            bool isAny = false;
            for (std::size_t i = 0; i < proposition->hedges.size(); ++i) {
                isAny |= proposition->hedges.at(i)->name() == Any().name();
                if (isAny) return 1.0;
            }
            InputVariable* inputVariable = dynamic_cast<InputVariable*> (proposition->variable);
            scalar result = proposition->term->membership(inputVariable->getInputValue());
            for (std::size_t i = 0; i < proposition->hedges.size(); ++i) {
                result = proposition->hedges.at(i)->hedge(result);
            }
            return result;
        }
        //if node is an operatorsk
        const Operator* fuzzyOperator =
                dynamic_cast<const Operator*> (node);
        if (not fuzzyOperator->left or not fuzzyOperator->right) {
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
        return this->activationDegree(conjunction, disjunction, this->_root);
    }

    void Antecedent::load(const std::string& antecedent, const Engine* engine) {
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
                if (engine->hasHedge(token)){
                    hedge = engine->getHedge(token);
                }else{
                    std::vector<std::string> hedges = FactoryManager::instance()->hedge()->available();
                    if (std::find(hedges.begin(), hedges.end(), token) != hedges.end()){
                        hedge = FactoryManager::instance()->hedge()->createInstance(token);
                        //TODO: find a better way, eventually.
                        const_cast<Engine*>(engine)->addHedge(hedge);
                    }
                }
                if (hedge) {
                    proposition->hedges.push_back(hedge);
                    if (dynamic_cast<Any*>(hedge)) {
                        state = S_VARIABLE bitor S_AND_OR;
                    } else {
                        state = S_HEDGE bitor S_TERM;
                    }
                    continue;
                }
            }

            if (state bitand S_TERM) {
                if (proposition->variable->hasTerm(token)) {
                    proposition->term =
                            proposition->variable->getTerm(token);
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
        this->_root = expressionStack.top();
    }

    std::string Antecedent::toString() const {
        return this->toInfix(this->_root);
    }

    std::string Antecedent::toPrefix(const Expression* node) const {
        if (not node)
            node = this->_root;
        if (not node->isOperator) { //is proposition
            return node->toString();
        }
        const Operator* fuzzyOperator =
                dynamic_cast<const Operator*> (node);
        std::stringstream ss;
        ss << fuzzyOperator->toString() << " "
                << this->toPrefix(fuzzyOperator->left) << " "
                << this->toPrefix(fuzzyOperator->right) << " ";
        return ss.str();
    }

    std::string Antecedent::toInfix(const Expression* node) const {
        if (not node)
            node = this->_root;
        if (not node->isOperator) { //is proposition
            return node->toString();
        }
        const Operator* fuzzyOperator =
                dynamic_cast<const Operator*> (node);
        std::stringstream ss;
        ss << this->toInfix(fuzzyOperator->left) << " "
                << fuzzyOperator->toString() << " "
                << this->toInfix(fuzzyOperator->right) << " ";
        return ss.str();
    }

    std::string Antecedent::toPostfix(const Expression* node) const {
        if (not node)
            node = this->_root;
        if (not node->isOperator) { //is proposition
            return node->toString();
        }
        const Operator* fuzzyOperator =
                dynamic_cast<const Operator*> (node);
        std::stringstream ss;
        ss << this->toPostfix(fuzzyOperator->left) << " "
                << this->toPostfix(fuzzyOperator->right) << " "
                << fuzzyOperator->toString() << " ";
        return ss.str();
    }


}
