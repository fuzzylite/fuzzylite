/*
 * MamdaniAntecedent.cpp
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "MamdaniAntecedent.h"

#include "../../engine/Engine.h"
#include "../../variable/InputVariable.h"
#include "../../hedge/Hedge.h"
#include "../../term/Term.h"

#include "../../rule/Rule.h"
#include "../Infix.h"
#include "../../engine/Operator.h"

#include "../../definitions.h"

#include "../../Example.h"

#include <stack>

namespace fl {

    MamdaniAntecedent::MamdaniAntecedent()
            : _root(NULL) {
    }

    MamdaniAntecedent::~MamdaniAntecedent() {
        if (_root)
            delete _root;
    }

    void MamdaniAntecedent::load(const std::string& antecedent, const Engine* engine) {
        /*
         Builds an proposition tree from the antecedent of a fuzzy rule.
         The rules are:
         1) After a variable comes 'is',
         2) After 'is' comes a hedge or a term
         3) After a hedge comes a hedge or a term
         4) After a term comes a variable or an operator
         */
        Infix infix;
        std::string postfix = infix.toPostfix(antecedent);
        std::stringstream tokenizer(postfix);
        std::string token;
        enum FSM {
            S_VARIABLE = 1, S_IS = 2, S_HEDGE = 4, S_TERM = 8, S_OPERATOR = 16
        };
        int state = S_VARIABLE;
        std::stack<Node*> nodeStack;
        PropositionNode* proposition = NULL;
        while (tokenizer >> token) {
            if (state bitand S_VARIABLE) {
                if (engine->hasInputVariable(token)) {
                    proposition = new PropositionNode;
                    nodeStack.push(proposition);
                    proposition->inputVariable = engine->getInputVariable(token);
                    state = S_IS;
                    continue;
                }
            }

            if (state bitand S_IS) {
                if (token != Rule::FL_IS) {
                    FL_LOG("expected keyword <" << Rule::FL_IS << ">, but found <" << token << ">");
                    throw std::exception();
                }
                state = S_HEDGE bitor S_TERM;
                continue;
            }

            if (state bitand S_HEDGE) {
                if (engine->hasHedge(token)) {
                    proposition->hedges.push_back(engine->getHedge(token));
                    if (token == Any().name()) {
                        state = S_VARIABLE bitor S_OPERATOR;
                    } else {
                        state = S_HEDGE bitor S_TERM;
                    }
                    continue;
                }
            }

            if (state bitand S_TERM) {
                if (proposition->inputVariable->hasTerm(token)) {
                    proposition->term =
                            proposition->inputVariable->getTerm(token);
                    state = S_VARIABLE bitor S_OPERATOR;
                    continue;
                }
            }

            if (state bitand S_OPERATOR) {
                if (infix.isOperator(token)) {
                    if (nodeStack.size() < 2) {
                        FL_LOG("operator <" << token << "> expected 2 operands,"
                                << "but found just " << nodeStack.size());
                        throw std::exception();
                    }
                    OperatorNode* operatorNode = new OperatorNode;
                    operatorNode->name = token;
                    operatorNode->right = nodeStack.top();
                    nodeStack.pop();
                    operatorNode->left = nodeStack.top();
                    nodeStack.pop();
                    nodeStack.push(operatorNode);

                    state = S_VARIABLE bitor S_OPERATOR;
                    continue;
                }
            }

            //If reached this point, there was an error
            if ((state bitand S_VARIABLE) or (state bitand S_OPERATOR)) {
                FL_LOG("expected input variable or operator, but found <" << token << ">");
                throw std::exception();
            }
            if ((state bitand S_HEDGE) or (state bitand S_TERM)) {
                FL_LOG("expected hedge or term, but found <" << token << ">");
                throw std::exception();
            }
            FL_LOG("unexpected token <" << token << ">");
            throw std::exception();
        }

        if (nodeStack.size() != 1) {
            FL_LOG("stack expected to contain the root, but contains "
                    << nodeStack.size() << " nodes");
            throw std::exception();
        }
        this->_root = nodeStack.top();
    }

    scalar MamdaniAntecedent::firingStrength(const Operator* tnorm, const Operator* snorm,
            const Node* node) const {
        if (not node->isOperator) { //is Proposition
            const PropositionNode* propositionNode =
                    dynamic_cast<const PropositionNode*>(node);
            scalar result =
                    propositionNode->term->membership(propositionNode->inputVariable->getInput());
            for (std::size_t i = 0; i < propositionNode->hedges.size(); ++i) {
                result = propositionNode->hedges[i]->hedge(result);
            }
            return result;
        }
        //if node is an operator
        const OperatorNode* operatorNode =
                dynamic_cast<const OperatorNode*>(node);
        if (not operatorNode->left  or  not operatorNode->right) {
            FL_LOG("left and right operands must exist");
            throw std::exception();
        }
        if (operatorNode->name == Rule::FL_AND)
            return tnorm->compute(
                    this->firingStrength(tnorm, snorm, operatorNode->left),
                    this->firingStrength(tnorm, snorm, operatorNode->right));

        if (operatorNode->name == Rule::FL_AND)
            return snorm->compute(
                    this->firingStrength(tnorm, snorm, operatorNode->left),
                    this->firingStrength(tnorm, snorm, operatorNode->right));
        FL_LOG("unknown operator <" << operatorNode->name << ">");
        throw std::exception();

    }
    scalar MamdaniAntecedent::firingStrength(const Operator* tnorm, const Operator* snorm) const {
        return this->firingStrength(tnorm, snorm, this->_root);
    }

    std::string MamdaniAntecedent::toString() const {
        return this->toStringPostfix(this->_root);
    }

    std::string MamdaniAntecedent::toStringPrefix(const Node* node) const {
        if (not node) node = this->_root;
        if (not node->isOperator) { //is proposition
            const PropositionNode* propositionNode =
                    dynamic_cast<const PropositionNode*>(node);
            return propositionNode->toString();
        }
        const OperatorNode* operatorNode =
                dynamic_cast<const OperatorNode*>(node);
        std::stringstream ss;
        ss << operatorNode->toString() << " "
                << this->toStringPrefix(operatorNode->left) << " "
                << this->toStringPrefix(operatorNode->right) << " ";
        return ss.str();
    }

    std::string MamdaniAntecedent::toStringInfix(const Node* node) const {
        if (not node) node = this->_root;
        if (not node->isOperator) { //is proposition
            const PropositionNode* propositionNode =
                    dynamic_cast<const PropositionNode*>(node);
            return propositionNode->toString();
        }
        const OperatorNode* operatorNode =
                dynamic_cast<const OperatorNode*>(node);
        std::stringstream ss;
        ss << this->toStringInfix(operatorNode->left) << " "
                << operatorNode->toString() << " "
                << this->toStringInfix(operatorNode->right) << " ";
        return ss.str();
    }

    std::string MamdaniAntecedent::toStringPostfix(const Node* node) const {
        if (not node) node = this->_root;
        if (not node->isOperator) { //is proposition
            const PropositionNode* propositionNode =
                    dynamic_cast<const PropositionNode*>(node);
            return propositionNode->toString();
        }
        const OperatorNode* operatorNode =
                dynamic_cast<const OperatorNode*>(node);
        std::stringstream ss;
        ss << this->toStringPostfix(operatorNode->left) << " "
                << this->toStringPostfix(operatorNode->right) << " "
                << operatorNode->toString() << " ";
        return ss.str();
    }

    void MamdaniAntecedent::main() {
        Engine* engine = Example::simpleMamdani();
        std::string antecedent =
                "(Energy is MEDIUM and Energy is LOW) or Energy is HIGH and Energy is LOW";
        MamdaniAntecedent m;
        m.load(antecedent, engine);
        FL_LOG(m.toStringInfix(m._root));
    }

} /* namespace fl */
