/* 
 * File:   FuzzyAction.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 8:54 PM
 */

#ifndef _FUZZYACTION_H
#define	_FUZZYACTION_H

#include "OutputLVar.h"
#include "Hedge.h"
#include "FuzzyOperator.h"
#include "DiscreteTerm.h"



namespace fuzzy_lite {

    class FuzzyAction {
    private:
        OutputLVar* _output_lvar;
        //std::vector<Hedge*> _hedges; //TODO: Ask if hedges apply in output
        const DiscreteTerm* _term;
        flScalar _probability;
    public:
        FuzzyAction();
        virtual ~FuzzyAction();

        virtual void execute(flScalar degree, const FuzzyOperator& fuzzy_op);

        virtual void setOutputLVar(OutputLVar* output_lvar);
        virtual void setTerm(const DiscreteTerm& term);
        virtual void setProbability(flScalar prob);
        
        virtual std::string toString() const;


    };
}

#endif	/* _FUZZYACTION_H */

