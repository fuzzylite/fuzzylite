/*
 * MamdaniAntecedent.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIANTECEDENT_H_
#define FL_MAMDANIANTECEDENT_H_

#include "../Antecedent.h"

#include <vector>

namespace fl {
    class Engine;
    class InputVariable;
    class Hedge;
    class Term;

    class MamdaniAntecedent: public Antecedent {

        class Node {
        public:
            const bool isOperator;
            Node(bool isOperator)
                    : isOperator(isOperator) {
            }
            virtual ~Node() {
            }

            virtual std::string toString() const = 0;
        };

        class OperatorNode: public Node {
        public:
            std::string name;
            Node* left;
            Node* right;

            OperatorNode()
                    : Node(true), left(NULL), right(NULL) {
            }
            ~OperatorNode() {
                if (left) delete left;
                if (right) delete right;
            }

        };

        class PropositionNode: public Node {
        public:
            InputVariable* inputVariable;
            std::vector<Hedge*> hedges;
            Term* term;
            PropositionNode()
                    : Node(false) {
            }
        };

    protected:
        Node* _root;

    public:
        MamdaniAntecedent();
        ~MamdaniAntecedent();

        void load(const std::string& antecedent, const Engine* engine);

        scalar firingStrength(const Operator* tnorm, const Operator* snorm, const Node* node) const;

        scalar firingStrength(const Operator* tnorm, const Operator* snorm) const;

        std::string toString() const;

        std::string toStringPrefix(const Node* node) const;
        std::string toStringInfix(const Node* node) const;
        std::string toStringPostfix(const Node* node) const;
    };

} /* namespace fl */
#endif /* FL_MAMDANIANTECEDENT_H_ */
