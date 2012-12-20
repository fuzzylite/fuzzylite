/*
 * Gaussian.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_GAUSSIAN_H_
#define FL_GAUSSIAN_H_

#include "fl/term/Term.h"

namespace fl {

    class Gaussian : public Term {
    protected:
        scalar _mean;
        scalar _sigma;
        scalar _minimum, _maximum;

    public:
        Gaussian(const std::string& name = "",
                scalar mean = std::numeric_limits<scalar>::quiet_NaN(),
                scalar sigma = -std::numeric_limits<scalar>::quiet_NaN(),
                scalar minimum = -std::numeric_limits<scalar>::infinity(),
                scalar maximum = std::numeric_limits<scalar>::infinity());
        ~Gaussian();

        std::string className() const;
        Gaussian* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setMean(scalar c);
        scalar getMean() const;

        void setSigma(scalar sigma);
        scalar getSigma() const;



        void setMinimum(scalar minimum);
        scalar minimum() const;

        void setMaximum(scalar maximum);
        scalar maximum() const;



    };

} /* namespace fl */
#endif /* FL_GAUSSIAN_H_ */
