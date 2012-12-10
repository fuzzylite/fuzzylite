/*
 * RightShoulder.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_RIGHTSHOULDER_H_
#define FL_RIGHTSHOULDER_H_

#include "fl/term/Term.h"

namespace fl {

    class RightShoulder: public Term {
    protected:
        scalar _minimum, _maximum;
        public:
        RightShoulder(const std::string& name = "",
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        ~RightShoulder();

        std::string className() const{
            return "RightShoulder";
        }

        scalar membership(scalar x) const;
        std::string toString() const;

        void setMinimum(scalar minimum);
        scalar minimum() const;

        void setMaximum(scalar maximum);
        scalar maximum() const;

    };

} /* namespace fl */
#endif /* FL_RIGHTSHOULDER_H_ */
