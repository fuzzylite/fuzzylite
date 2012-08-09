/* 
 * File:   ContinuousTerm.h
 * Author: jcrada
 *
 * Created on November 2, 2009, 1:57 PM
 */

#ifndef _CONTINUOUSTERM_H
#define	_CONTINUOUSTERM_H

#include <string>
#include "flScalar.h"
namespace fuzzy_lite {
//TODO: Aggregate (list<ContinuousTerms>), Modulate(topvalue ), Membership(for all aggregate(membership(x)).
    class ContinuousTerm {
    public:
        virtual ~ContinuousTerm() = 0;
        virtual std::string getName() const = 0;
        virtual flScalar getMinimumValue() const = 0;
        virtual flScalar getMaximumValue() const = 0;
        virtual flScalar getResolution() const = 0;
        virtual flScalar degreeOfMembership(flScalar crisp) const = 0;
    };
}

#endif	/* _CONTINUOUSTERM_H */

