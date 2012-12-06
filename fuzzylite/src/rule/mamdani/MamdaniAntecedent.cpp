/*
 * MamdaniAntecedent.cpp
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#include "MamdaniAntecedent.h"

#include "../../variable/InputVariable.h"
#include "../../hedge/Hedge.h"
#include "../../term/Term.h"

#include "../../rule/Rule.h"
#include "../Infix.h"
#include "../../engine/Operator.h"

#include "../../definitions.h"

namespace fl {

    MamdaniAntecedent::MamdaniAntecedent()
            : _root(NULL) {
    }

    MamdaniAntecedent::~MamdaniAntecedent() {
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
        Infix* infix = new Infix;
        std::string postfix = infix->toPostfix(antecedent);
        std::stringstream tokenizer(postfix);
        std::string token;
        enum FSM{
            VARIABLE = 1, IS = 2, HEDGE = 4, TERM = 8, OPERATOR = 16
        };
        int state = VARIABLE;

    }

    scalar MamdaniAntecedent::firingStrength(const Operator* tnorm, const Operator* snorm,
            const Node* node) const {
        if (!node->isOperator) { //is Proposition
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
        if (!operatorNode->left || !operatorNode->right) {
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
        if (!node->isOperator) { //is proposition
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
        if (!node->isOperator) { //is proposition
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
        if (!node->isOperator) { //is proposition
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

} /* namespace fl */
