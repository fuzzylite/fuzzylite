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
                scalar meanA = fl::nan,
                scalar standardDeviationA = fl::nan,
                scalar meanB = fl::nan,
                scalar standardDeviationB = fl::nan);

        virtual ~GaussianProduct();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;

        virtual void setMeanA(scalar meanA);
        virtual scalar getMeanA() const;

        virtual void setStandardDeviationA(scalar sigmaA);
        virtual scalar getStandardDeviationA() const;

        virtual void setMeanB(scalar meanB);
        virtual scalar getMeanB() const;

        virtual void setStandardDeviationB(scalar sigmaB);
        virtual scalar getStandardDeviationB() const;
        
        virtual GaussianProduct* copy() const;
        static Term* constructor();
    };
}
#endif	/* FL_GAUSSIANPRODUCT_H */

