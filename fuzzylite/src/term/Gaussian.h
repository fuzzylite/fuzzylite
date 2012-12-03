/*
 * Gaussian.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_GAUSSIAN_H_
#define FL_GAUSSIAN_H_

#include "Term.h"

namespace fl {

    class Gaussian: public Term {
    protected:
        scalar _sigma;
        scalar _c;
        scalar _minimum, _maximum;

    public:
        Gaussian(const std::string& name,
                scalar sigma = -std::numeric_limits<scalar>::quiet_NaN(),
                scalar c = std::numeric_limits<scalar>::quiet_NaN(),
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        ~Gaussian();

        scalar membership(scalar x) const;
        std::string toString() const;

        void setSigma(scalar sigma);
        scalar getSigma() const;

        void setC(scalar c);
        scalar getC() const;

        void setMinimum(scalar minimum);
        scalar minimum() const;

        void setMaximum(scalar maximum);
        scalar maximum() const;

    };

} /* namespace fl */
#endif /* FL_GAUSSIAN_H_ */
