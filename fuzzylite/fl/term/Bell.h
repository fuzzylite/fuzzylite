/*
 * Bell.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_BELL_H_
#define FL_BELL_H_

#include "fl/term/Term.h"

namespace fl {

    class Bell: public Term {
    protected:
        scalar _a;
        scalar _b;
        scalar _c;
        scalar _minimum, _maximum;
        public:
        Bell(const std::string& name = "",
                scalar a = std::numeric_limits<scalar>::quiet_NaN(),
                scalar b = std::numeric_limits<scalar>::quiet_NaN(),
                scalar c = std::numeric_limits<scalar>::quiet_NaN(),
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        ~Bell();

        std::string className() const;
        Bell* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;

        void setA(scalar a);
        scalar getA() const;

        void setB(scalar b);
        scalar getB() const;

        void setC(scalar c);
        scalar getC() const;

        void setMinimum(scalar minimum);
        scalar minimum() const;

        void setMaximum(scalar maximum);
        scalar maximum() const;

    }
    ;

} /* namespace fl */
#endif /* FL_BELL_H_ */
