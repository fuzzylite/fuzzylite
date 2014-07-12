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
 * Term.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_TERM_H
#define FL_TERM_H

#include "fl/fuzzylite.h"

#include "fl/Operation.h"

#include <cmath>
#include <string>
#include <vector>

namespace fl {
    class Engine;

    class FL_EXPORT Term {
    protected:
        std::string _name;
        scalar _height;
    public:

        Term(const std::string& name = "", scalar height = 1.0);
        Term(const Term& copy);
        Term& operator=(const Term& rhs);
        virtual ~Term();

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        virtual void setHeight(scalar height);
        virtual scalar getHeight() const;

        virtual std::string toString() const;

        virtual std::string className() const = 0;
        virtual std::string parameters() const = 0;
        virtual void configure(const std::string& parameters) = 0;

        virtual scalar membership(scalar x) const = 0;

        virtual Term* clone() const = 0;

        static void updateReference(Term* term, const Engine* engine);
    };

}
#endif /* FL_TERM_H */
