/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * File:   Ramp.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 4:27 PM
 */

#ifndef FL_RAMP_H
#define FL_RAMP_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Ramp : public Term {
    protected:
        scalar _start, _end;

    public:

        enum Direction {
            POSITIVE, ZERO, NEGATIVE
        };
        Ramp(const std::string& name = "",
                scalar start = fl::nan,
                scalar end = fl::nan,
                scalar height = 1.0);
        virtual ~Ramp();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;

        virtual void setStart(scalar start);
        virtual scalar getStart() const;

        virtual void setEnd(scalar end);
        virtual scalar getEnd() const;

        virtual Direction direction() const;

        virtual Ramp* clone() const;

        static Term* constructor();
    };
}
#endif  /* FL_RAMP_H */

