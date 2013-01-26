/*
 * Gaussian.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_GAUSSIAN_H
#define FL_GAUSSIAN_H

#include "fl/term/Term.h"

namespace fl {

    flclass Gaussian : public Term {
    protected:
        scalar _mean;
        scalar _standardDeviation;

    public:
        Gaussian(const std::string& name = "",
                scalar mean = std::numeric_limits<scalar>::quiet_NaN(),
                scalar sigma = -std::numeric_limits<scalar>::quiet_NaN());
        ~Gaussian();

        std::string className() const;
        Gaussian* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setMean(scalar c);
        scalar getMean() const;

        void setStandardDeviation(scalar sigma);
        scalar getStandardDeviation() const;

    };

} 
#endif /* FL_GAUSSIAN_H */
