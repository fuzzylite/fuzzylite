/* 
 * File:   FuzzyProposition.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 6:19 PM
 */

#ifndef _FUZZYPROPOSITION_H
#define	_FUZZYPROPOSITION_H

#include "InputLVar.h"
#include "Hedge.h"
#include "FuzzyOperator.h"
#include "DiscreteTerm.h"


namespace fuzzy_lite {

    class FuzzyProposition {
    public:

        enum OPERATION {
            O_NONE = ' ',
            O_AND = '&',
            O_OR = '*',
        };
    private:
        FuzzyProposition* _left;
        FuzzyProposition* _right;
        OPERATION _operation;
        const InputLVar* _input_lvar; //e.g. power
        std::vector<Hedge*> _hedges; //very
        const DiscreteTerm* _term; //high

    public:
        FuzzyProposition();
        virtual ~FuzzyProposition();

        virtual void setLeft(FuzzyProposition& proposition);
        virtual void setRight(FuzzyProposition& proposition);
        virtual void setOperation(OPERATION operation);
        virtual void setInputLVar(const InputLVar& lvar);
        virtual void addHedge(Hedge& hedge);
        virtual void setTerm(const DiscreteTerm& term);

        virtual bool isTerminal() const;

        virtual flScalar degreeOfTruth(const FuzzyOperator& fuzzy_op) const;

        virtual std::string toString() const;

    };
}

#endif	/* _FUZZYPROPOSITION_H */

