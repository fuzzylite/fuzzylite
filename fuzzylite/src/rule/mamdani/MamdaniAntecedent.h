/*
 * MamdaniAntecedent.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIANTECEDENT_H_
#define FL_MAMDANIANTECEDENT_H_

#include "../Antecedent.h"

#include "../Rule.h"
#include "../../variable/InputVariable.h"
#include "../../hedge/Hedge.h"
#include "../../term/Term.h"



#include <sstream>
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
                if (left)
                    delete left;
                if (right)
                    delete right;
            }
            std::string toString() const {
                return name;
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
            std::string toString() const {
                std::stringstream ss;
                ss << inputVariable->getName() << " " << Rule::FL_IS << " ";
                for (std::size_t i = 0; i < hedges.size(); ++i) {
                    ss << hedges[i]->name();
                    if (i < hedges.size() - 1)
                        ss << " ";
                }
                ss << term->getName();
                return ss.str();
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

        static void main();
    };

} /* namespace fl */
#endif /* FL_MAMDANIANTECEDENT_H_ */
