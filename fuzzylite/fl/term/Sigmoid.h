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
 * Sigmoid.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_SIGMOID_H
#define FL_SIGMOID_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Sigmoid : public Term {
    protected:
        scalar _inflection;
        scalar _slope;
    public:
        Sigmoid(const std::string& name = "",
                scalar inflection = fl::nan,
                scalar slope = fl::nan);
        virtual ~Sigmoid();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);
        
        virtual scalar membership(scalar x) const;

        virtual void setInflection(scalar inflection);
        virtual scalar getInflection() const;

        virtual void setSlope(scalar slope);
        virtual scalar getSlope() const;
        
        virtual Sigmoid* copy() const;
        
        static Term* constructor();
    };

}
#endif /* FL_SIGMOID_H */
