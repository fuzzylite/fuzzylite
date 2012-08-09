/* 
 * File:   LinguisticVariable.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 1:56 PM
 */

#ifndef _LINGUISTICVARIABLE_H
#define	_LINGUISTICVARIABLE_H

#include <string>
#include <vector>

#include "DiscreteTerm.h"
#include "defs.h"

namespace fuzzy_lite {

    class LinguisticVariable {
    public:

        enum MEMBERSHIP_FUNCTION {
            MF_TRIANGULAR = 1, MF_TRAPEZOIDAL,
        };
    private:
        std::string _name;
        std::vector<DiscreteTerm*> _terms;
    protected:
        virtual int positionFor(flScalar value);
    public:
        LinguisticVariable();
        LinguisticVariable(const std::string& name);
        virtual ~LinguisticVariable();

        virtual std::string getName() const;
        virtual void setName(const std::string& name);

        virtual void addTerm(DiscreteTerm& term);
        virtual DiscreteTerm* removeTerm(int index) throw (OutOfRangeException);
        virtual DiscreteTerm* removeTerm(const std::string& name);
        virtual DiscreteTerm& getTerm(int index) const throw (OutOfRangeException);
        virtual DiscreteTerm* getTerm(const std::string& name) const;
        virtual DiscreteTerm* firstTerm() const;
        virtual DiscreteTerm* lastTerm() const;
        virtual bool isEmpty() const;
        virtual int numberOfTerms() const;

        virtual flScalar minimum() const;
        virtual flScalar maximum() const;

        virtual DiscreteTerm compound() const;

        virtual std::string fuzzify(flScalar crisp) const;

        virtual DiscreteTerm* bestFuzzyApproximation(flScalar crisp);

        virtual std::string toString() const;

        virtual void autoCreateTerms(flScalar min, flScalar max, int terms,
                std::vector<std::string>& names, MEMBERSHIP_FUNCTION mf,
                bool from_infinity, bool to_infinity) throw (InvalidArgumentException);

    };
}

#endif	/* _LINGUISTICVARIABLE_H */

