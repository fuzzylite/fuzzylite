/*
 * Sigmoid.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_SIGMOID_H_
#define FL_SIGMOID_H_

#include "Term.h"

namespace fl {

    class Sigmoid: public Term {
    protected:
        scalar _a;
        scalar _c;
        scalar _minimum, _maximum;
        public:
        Sigmoid(const std::string& name,
                scalar a = std::numeric_limits<scalar>::quiet_NaN(),
                scalar c = std::numeric_limits<scalar>::quiet_NaN(),
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        ~Sigmoid();

        scalar membership(scalar x);

        std::string toString() const;

        void setA(scalar a);
        scalar getA() const;

        void setC(scalar c);
        scalar getC() const;

        void setMinimum(scalar minimum);
        scalar minimum() const;

        void setMaximum(scalar maximum);
        scalar maximum() const;

    };

} /* namespace fl */
#endif /* FL_SIGMOID_H_ */
