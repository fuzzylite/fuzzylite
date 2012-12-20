/*
 * LeftShoulder.h
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#ifndef FL_LEFTSHOULDER_H_
#define FL_LEFTSHOULDER_H_

#include "fl/term/Term.h"

namespace fl {

    class LeftShoulder : public Term {
    protected:
        scalar _minimum, _maximum;

    public:
        LeftShoulder(const std::string& name = "",
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        ~LeftShoulder();

        std::string className() const;
        LeftShoulder* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setMinimum(scalar minimum);
        scalar minimum() const;

        void setMaximum(scalar maximum);
        scalar maximum() const;

    };
}

#endif /* FL_LEFTSHOULDER_H_ */
