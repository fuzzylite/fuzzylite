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
 * Triangle.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_TRIANGLE_H
#define FL_TRIANGLE_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Triangle : public Term {
    protected:
        scalar _vertexA;
        scalar _vertexB;
        scalar _vertexC;
    public:
        Triangle(const std::string& name = "",
                scalar vertexA = fl::nan,
                scalar vertexB = fl::nan,
                scalar vertexC = fl::nan,
                scalar height = 1.0);
        virtual ~Triangle();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;

        virtual void setVertexA(scalar a);
        virtual scalar getVertexA() const;

        virtual void setVertexB(scalar b);
        virtual scalar getVertexB() const;

        virtual void setVertexC(scalar c);
        virtual scalar getVertexC() const;

        virtual Triangle* clone() const;

        static Term* constructor();

    };

}
#endif /* FL_TRIANGLE_H */
