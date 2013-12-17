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
                scalar a = -fl::inf,
                scalar b = fl::nan,
                scalar c = fl::nan,
                scalar d = fl::inf);
        virtual ~Trapezoid();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;

        virtual void setA(scalar a);
        virtual scalar getA() const;

        virtual void setB(scalar b);
        virtual scalar getB() const;

        virtual void setC(scalar c);
        virtual scalar getC() const;

        virtual void setD(scalar d);
        virtual scalar getD() const;

        virtual Trapezoid* copy() const;
        
        static Term* constructor();
    };

}
#endif /* FL_TRAPEZOID_H */
