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
 * Gaussian.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_GAUSSIAN_H
#define FL_GAUSSIAN_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Gaussian : public Term {
    protected:
        scalar _mean;
        scalar _standardDeviation;

    public:
        Gaussian(const std::string& name = "",
                scalar mean = fl::nan,
                scalar standardDeviation = fl::nan);
        virtual ~Gaussian();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);
        
        virtual scalar membership(scalar x) const;

        virtual void setMean(scalar c);
        virtual scalar getMean() const;

        virtual void setStandardDeviation(scalar sigma);
        virtual scalar getStandardDeviation() const;
        
        virtual Gaussian* copy() const;
        static Term* constructor();
    };

}
#endif /* FL_GAUSSIAN_H */
