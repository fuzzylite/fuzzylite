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
 * Trapezoid.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_TRAPEZOID_H
#define FL_TRAPEZOID_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Trapezoid : public Term {
    protected:
        scalar _a, _b, _c, _d;
    public:
        Trapezoid(const std::string& name = "",
                scalar a = -std::numeric_limits<scalar>::infinity(),
                scalar b = std::numeric_limits<scalar>::quiet_NaN(),
                scalar c = std::numeric_limits<scalar>::quiet_NaN(),
                scalar d = std::numeric_limits<scalar>::infinity());
        ~Trapezoid();

        std::string className() const;
        Trapezoid* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setA(scalar a);
        scalar getA() const;

        void setB(scalar b);
        scalar getB() const;

        void setC(scalar c);
        scalar getC() const;

        void setD(scalar d);
        scalar getD() const;

    };

} 
#endif /* FL_TRAPEZOID_H */
