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
 * Defuzzifier.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */


//TODO Check http://en.wikipedia.org/wiki/Defuzzification for other defuzzifiers.

#ifndef FL_DEFUZZIFIER_H
#define FL_DEFUZZIFIER_H

#include "fl/fuzzylite.h"
#include <string>

namespace fl {
    class Term;

    class FL_EXPORT Defuzzifier {
    public:

        Defuzzifier() {
        }

        virtual ~Defuzzifier() {
        }

        virtual std::string className() const = 0;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const = 0;

    };

}
#endif /* FL_DEFUZZIFIER_H */
