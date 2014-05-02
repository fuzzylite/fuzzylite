// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

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
                scalar a = fl::nan,
                scalar b = fl::nan,
                scalar c = fl::nan,
                scalar d = fl::nan);
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

        virtual Trapezoid* clone() const;

        static Term* constructor();
    };

}
#endif /* FL_TRAPEZOID_H */
