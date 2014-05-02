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
 * Term.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_TERM_H
#define FL_TERM_H

#include "fl/fuzzylite.h"

#include "fl/Operation.h"
#include "fl/factory/FactoryManager.h"

#include <string>
#include <limits>
#include <iomanip>
#include <cmath>
#include <vector>

namespace fl {

    class FL_EXPORT Term {
    protected:
        std::string _name;

    public:

        Term(const std::string& name = "");
        Term(const Term& copy);
        Term& operator=(const Term& rhs);
        virtual ~Term();

        /**
         * Sets the name of this term. It <b>must</b> be a valid id (see Op::makeValidId)
         * @param name
         */
        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        virtual std::string toString() const;

        virtual std::string className() const = 0;
        virtual std::string parameters() const = 0;
        virtual void configure(const std::string& parameters) = 0;

        virtual scalar membership(scalar x) const = 0;

        virtual Term* clone() const = 0;

    };

}
#endif /* FL_TERM_H */
