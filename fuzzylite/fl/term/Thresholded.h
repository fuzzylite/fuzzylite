/*
 * Thresholded.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_THRESHOLDED_H_
#define FL_THRESHOLDED_H_

#include "fl/term/Term.h"

namespace fl {
    class Operator;

    class Thresholded : public Term {
    protected:
        const Term* _term;
        scalar _threshold;
        const Operator* _activation;

    public:
        Thresholded(const Term* term = NULL, scalar threshold = 1.0,
                const Operator* activationOperator = NULL);

        ~Thresholded();

        std::string className() const;
        Thresholded* copy() const;

        scalar membership(scalar x) const;
        scalar minimum() const;
        scalar maximum() const;

        void setTerm(const Term* term);
        const Term* getTerm() const;

        void setThreshold(scalar threshold);
        scalar getThreshold() const;

        void setActivation(const Operator* activation);
        const Operator* getActivation() const;

        std::string toString() const;

    };

} /* namespace fl */
#endif /* FL_THRESHOLDED_H_ */
