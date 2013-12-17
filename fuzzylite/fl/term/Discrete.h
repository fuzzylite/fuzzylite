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
 * Discrete.h
 *
 *  Created on: 5/12/2012
 *      Author: jcrada
 */

#ifndef FL_DISCRETE_H
#define FL_DISCRETE_H

#include "fl/term/Term.h"

#include "fl/Exception.h"

#include <vector>
#include <utility>

namespace fl {

    class FL_EXPORT Discrete : public Term {
    public:
        std::vector<scalar> x;
        std::vector<scalar> y;

        Discrete(const std::string& name = "");

        Discrete(const std::string& name,
                const std::vector<scalar>& x,
                const std::vector<scalar>& y);

        Discrete(const std::string& name,
                const std::vector<std::pair<scalar, scalar> >& xy);

        virtual ~Discrete();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        //Warning: this method is unsafe. Make sure you use it correctly.
        template <typename T>
        static Discrete* create(const std::string& name, int argc,
                T x1, T y1, ...) throw (fl::Exception);

        virtual scalar membership(scalar x) const;

        virtual Discrete* copy() const;

        static Term* constructor();

    };

}
#endif /* FL_DISCRETE_H */
