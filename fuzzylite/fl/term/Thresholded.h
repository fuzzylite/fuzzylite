/*
 * Thresholded.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_THRESHOLDED_H
#define FL_THRESHOLDED_H

#include "fl/term/Term.h"

namespace fl {
    class TNorm;

    class FL_EXPORT Thresholded : public Term {
    protected:
        const Term* _term;
        scalar _threshold;
        const TNorm* _activation;

    public:
        Thresholded(const Term* term = NULL, scalar threshold = 1.0,
                const TNorm* activationOperator = NULL);

        ~Thresholded();

        std::string className() const;
        Thresholded* copy() const;

        scalar membership(scalar x) const;

        void setTerm(const Term* term);
        const Term* getTerm() const;

        void setThreshold(scalar threshold);
        scalar getThreshold() const;

        void setActivation(const TNorm* activation);
        const TNorm* getActivation() const;

        std::string toString() const;

    };

} 
#endif /* FL_THRESHOLDED_H */
