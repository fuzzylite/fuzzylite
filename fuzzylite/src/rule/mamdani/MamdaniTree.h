/*
 * MamdaniTree.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANITREE_H_
#define FL_MAMDANITREE_H_

#include "../../scalar.h"

#include <vector>
#include <string>

namespace fl {
    class Variable;
    class Hedge;
    class Term;

    class MamdaniNode {
    public:
        MamdaniNode(){}
        virtual ~MamdaniNode(){}
        virtual bool isProposition() const = 0;
        virtual bool isOperator() const = 0;
    };

    class MamdaniOperator: public MamdaniNode {
    protected:
        std::string _operator;
        MamdaniNode* _left;
        MamdaniNode* _right;
    public:

        bool isProposition() const {
            return false;
        }
        bool isOperator() const {
            return true;
        }

    };

    class MamdaniProposition: public MamdaniNode {
    protected:
        Variable* _variable;
        std::vector<Hedge*> _hedges;
        Term* _term;
        scalar _weight;
        public:
        MamdaniProposition();
        virtual ~MamdaniProposition();

        bool isProposition() const {
            return true;
        }
        bool isOperator() const {
            return false;
        }

        virtual void setVariable(Variable* variable);
        virtual Variable* getVariable() const;

        virtual void setTerm(Term* term);
        virtual Term* getTerm() const;

        virtual void setWeight(scalar weight);
        virtual scalar getWeight() const;

        virtual std::string toString() const;

        /**
         * Operations for iterable datatype _hedges
         */
        virtual void addHedge(Hedge* hedge);
        virtual void insertHedge(Hedge* hedge, int index);
        virtual Hedge* getHedge(int index) const;
        virtual Hedge* removeHedge(int index);
        virtual int numberOfHedges() const;
        virtual const std::vector<Hedge*>& hedges() const;
    };

} /* namespace fl */
#endif /* FL_MAMDANITREE_H_ */
