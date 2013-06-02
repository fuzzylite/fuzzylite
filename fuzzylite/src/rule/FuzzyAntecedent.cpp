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
 * FuzzyAntecedent.cpp
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "fl/rule/FuzzyAntecedent.h"

#include "fl/rule/FuzzyExpression.h"

#include "fl/Engine.h"
#include "fl/variable/InputVariable.h"
#include "fl/hedge/Hedge.h"
#include "fl/hedge/Any.h"
#include "fl/term/Term.h"

#include "fl/rule/Rule.h"
#include "fl/norm/TNorm.h"
#include "fl/norm/SNorm.h"

#include "fl/term/Function.h"



#include <stack>

namespace fl {

    FuzzyAntecedent::FuzzyAntecedent()
    : _root(NULL) { }

    FuzzyAntecedent::~FuzzyAntecedent() {
        if (_root) delete _root;
    }

    FuzzyExpression* FuzzyAntecedent::getRoot() const {
        return this->_root;
    }

    scalar FuzzyAntecedent::firingStrength(const TNorm* tnorm, const SNorm* snorm,
            const FuzzyExpression* node) const {
        if (not node->isOperator) { //is Proposition
            const FuzzyProposition* proposition =
                    dynamic_cast<const FuzzyProposition*> (node);
            bool isAny = false;
            for (std::size_t i = 0; i < proposition->hedges.size(); ++i) {
                isAny |= proposition->hedges.at(i)->name() == Any().name();
                if (isAny) return 1.0;
            }
            InputVariable* inputVariable = dynamic_cast<InputVariable*> (proposition->variable);
            scalar result = proposition->term->membership(inputVariable->getInput());
            for (std::size_t i = 0; i < proposition->hedges.size(); ++i) {
                result = proposition->hedges.at(i)->hedge(result);
            }
            return result;
        }
        //if node is an operatorsk
        const FuzzyOperator* fuzzyOperator =
                dynamic_cast<const FuzzyOperator*> (node);
        if (not fuzzyOperator->left or not fuzzyOperator->right) {
            std::ostringstream ex;
            ex << "[syntax error] left and right operands must exist";
            throw fl::Exception(ex.str(), FL_AT);
        }
        if (fuzzyOperator->name == Rule::FL_AND)
            return tnorm->compute(
                this->firingStrength(tnorm, snorm, fuzzyOperator->left),
                this->firingStrength(tnorm, snorm, fuzzyOperator->right));

        if (fuzzyOperator->name == Rule::FL_OR)
            return snorm->compute(
                this->firingStrength(tnorm, snorm, fuzzyOperator->left),
                this->firingStrength(tnorm, snorm, fuzzyOperator->right));
        std::ostringstream ex;
        ex << "[syntax error] operator <" << fuzzyOperator->name << "> not recognized";
        throw fl::Exception(ex.str(), FL_AT);

    }

    scalar FuzzyAntecedent::firingStrength(const TNorm* tnorm, const SNorm* snorm) const {
        return this->firingStrength(tnorm, snorm, this->_root);
    }

    void FuzzyAntecedent::load(const std::string& antecedent, const Engine* engine) {
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
            S_VARIABLE = 1, S_IS = 2, S_HEDGE = 4, S_TERM = 8, S_OPERATOR = 16
        };
        int state = S_VARIABLE;
        std::stack<FuzzyExpression*> expressionStack;
        FuzzyProposition* proposition = NULL;
        while (tokenizer >> token) {
            if (state bitand S_VARIABLE) {
                if (engine->hasInputVariable(token)) {
                    proposition = new FuzzyProposition;
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
                if (engine->hasHedge(token)) {
                    Hedge* hedge = engine->getHedge(token);
                    proposition->hedges.push_back(hedge);
                    if (token == Any().name()) {
                        state = S_VARIABLE bitor S_OPERATOR;
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
                    state = S_VARIABLE bitor S_OPERATOR;
                    continue;
                }
            }

            if (state bitand S_OPERATOR) {
                if (function.isOperator(token)) {
                    if (expressionStack.size() < 2) {
                        std::ostringstream ex;
                        ex << "[syntax error] operator <" << token << "> expects 2 operands,"
                                << "but found " << expressionStack.size();
                        throw fl::Exception(ex.str(), FL_AT);
                    }
                    FuzzyOperator* fuzzyOperator = new FuzzyOperator;
                    fuzzyOperator->name = token;
                    fuzzyOperator->right = expressionStack.top();
                    expressionStack.pop();
                    fuzzyOperator->left = expressionStack.top();
                    expressionStack.pop();
                    expressionStack.push(fuzzyOperator);

                    state = S_VARIABLE bitor S_OPERATOR;
                    continue;
                }
            }

            //If reached this point, there was an error
            if ((state bitand S_VARIABLE) or (state bitand S_OPERATOR)) {
                std::ostringstream ex;
                ex << "[syntax error] expected input variable or operator, but found <" << token << ">";
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

    std::string FuzzyAntecedent::toString() const {
        return this->toStringInfix(this->_root);
    }

    std::string FuzzyAntecedent::toStringPrefix(const FuzzyExpression* node) const {
        if (not node)
            node = this->_root;
        if (not node->isOperator) { //is proposition
            return node->toString();
        }
        const FuzzyOperator* fuzzyOperator =
                dynamic_cast<const FuzzyOperator*> (node);
        std::stringstream ss;
        ss << fuzzyOperator->toString() << " "
                << this->toStringPrefix(fuzzyOperator->left) << " "
                << this->toStringPrefix(fuzzyOperator->right) << " ";
        return ss.str();
    }

    std::string FuzzyAntecedent::toStringInfix(const FuzzyExpression* node) const {
        if (not node)
            node = this->_root;
        if (not node->isOperator) { //is proposition
            return node->toString();
        }
        const FuzzyOperator* fuzzyOperator =
                dynamic_cast<const FuzzyOperator*> (node);
        std::stringstream ss;
        ss << this->toStringInfix(fuzzyOperator->left) << " "
                << fuzzyOperator->toString() << " "
                << this->toStringInfix(fuzzyOperator->right) << " ";
        return ss.str();
    }

    std::string FuzzyAntecedent::toStringPostfix(const FuzzyExpression* node) const {
        if (not node)
            node = this->_root;
        if (not node->isOperator) { //is proposition
            return node->toString();
        }
        const FuzzyOperator* fuzzyOperator =
                dynamic_cast<const FuzzyOperator*> (node);
        std::stringstream ss;
        ss << this->toStringPostfix(fuzzyOperator->left) << " "
                << this->toStringPostfix(fuzzyOperator->right) << " "
                << fuzzyOperator->toString() << " ";
        return ss.str();
    }


}
