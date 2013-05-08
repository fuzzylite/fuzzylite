/* 
 * File:   Weighted.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 6:07 PM
 */

#ifndef FL_WEIGHTED_H
#define	FL_WEIGHTED_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Weighted : public Term {
    protected:
        const Term* _term;
        scalar _weight;
    public:
        Weighted(const std::string& name = "",
                const Term* term = NULL,
                scalar weight = 1.0);
        virtual ~Weighted();

        virtual void setTerm(const Term* term);
        virtual const Term* getTerm() const;

        virtual void setWeight(scalar weight);
        virtual scalar getWeight() const;

        virtual scalar membership(scalar x) const;

        virtual std::string className() const;
        virtual std::string toString() const;

        virtual Weighted* copy() const;
    };
}

#endif	/* FL_WEIGHTED_H */

