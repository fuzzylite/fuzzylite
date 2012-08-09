/* 
 * File:   FuzzyRule.h
 * Author: jcrada
 *
 * Created on November 5, 2009, 11:11 AM
 */

#ifndef _FUZZYRULE_H
#define	_FUZZYRULE_H

#include "FuzzyOperator.h"

#include "FuzzyEngine.h"

#include <string>

namespace fuzzy_lite {
    class FuzzyEngine; //forward declaration

    class FuzzyRule {
    public:
        virtual ~FuzzyRule(){};
        virtual bool isValid() const = 0;
        virtual flScalar evaluate(const FuzzyOperator& fuzzy_op) const = 0;
        virtual void fire(flScalar degree,const FuzzyOperator& fuzzy_op) = 0;

        virtual void parse(const std::string& rule, const FuzzyEngine& engine) = 0;
        virtual std::string toString() const = 0;
    };

     

}

#endif	/* _FUZZYRULE_H */

