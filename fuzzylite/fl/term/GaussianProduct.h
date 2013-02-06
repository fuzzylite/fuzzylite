/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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

