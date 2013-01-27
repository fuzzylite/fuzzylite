/* 
 * File:   GaussianProduct.h
 * Author: jcrada
 *
 * Created on 30 December 2012, 6:05 PM
 */

#ifndef FL_GAUSSIANPRODUCT_H
#define	FL_GAUSSIANPRODUCT_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT GaussianProduct : public Term {
    protected:
        scalar _meanA;
        scalar _standardDeviationA;
        scalar _meanB;
        scalar _standardDeviationB;

    public:
        GaussianProduct(const std::string& name = "",
                scalar meanA = std::numeric_limits<scalar>::quiet_NaN(),
                scalar sigmaA = std::numeric_limits<scalar>::quiet_NaN(),
                scalar meanB = std::numeric_limits<scalar>::quiet_NaN(),
                scalar sigmaB = std::numeric_limits<scalar>::quiet_NaN());

        std::string className() const;
        GaussianProduct* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;

        void setMeanA(scalar meanA);
        scalar getMeanA() const;

        void setStandardDeviationA(scalar sigmaA);
        scalar getStandardDeviationA() const;

        void setMeanB(scalar meanB);
        scalar getMeanB() const;

        void setStandardDeviationB(scalar sigmaB);
        scalar getStandardDeviationB() const;




    };
}
#endif	/* FL_GAUSSIANPRODUCT_H */

