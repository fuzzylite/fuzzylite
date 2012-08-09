/* 
 * File:   OutputLVar.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 2:30 PM
 */

#ifndef _OUTPUTLVAR_H
#define	_OUTPUTLVAR_H

#include "LinguisticVariable.h"
#include "DiscreteTerm.h"

namespace fuzzy_lite {

    class OutputLVar : public LinguisticVariable {
    private:
        DiscreteTerm* _output;
    public:
        OutputLVar();
        OutputLVar(const std::string& name);
        virtual ~OutputLVar();

        virtual const DiscreteTerm& getOutput() const;
        virtual flScalar getDefuzzifiedOutput(const FuzzyOperator& op) const;

        virtual void aggregateToOutput(const DiscreteTerm& term);
        virtual void clearOutput();

    };
}


#endif	/* _OUTPUTLVAR_H */

