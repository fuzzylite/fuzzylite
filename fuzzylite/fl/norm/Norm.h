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
 * File:   Norm.h
 * Author: jcrada
 *
 * Created on 7 April 2013, 10:06 PM
 */

#ifndef FL_NORM_H
#define	FL_NORM_H

#include "fl/fuzzylite.h"

#include "fl/Operation.h"

#include <string>

namespace fl {

    class FL_EXPORT Norm {
    public:

        Norm() {
        }

        virtual ~Norm() {
        }

        virtual std::string className() const = 0;
        virtual scalar compute(scalar a, scalar b) const = 0;

    };
}
#endif	/* FL_NORM_H */

