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
 * File:   ZShape.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:07 PM
 */

#ifndef FL_ZSHAPE_H
#define	FL_ZSHAPE_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT ZShape : public Term {
    protected:
        scalar _start, _end;

    public:
        ZShape(const std::string& name = "",
                scalar _start = -fl::inf,
                scalar _end = fl::inf);

        virtual ~ZShape();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;

        virtual void setStart(scalar start);
        virtual scalar getStart() const;

        virtual void setEnd(scalar end);
        virtual scalar getEnd() const;

        virtual ZShape* clone() const;
        
        static Term* constructor();

    };
}
#endif	/* ZSHAPE_H */

