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
 * File:   Maximum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:14 AM
 */

#include "fl/norm/s/Maximum.h"

namespace fl {

    std::string Maximum::className() const {
        return "Maximum";
    }

    scalar Maximum::compute(scalar a, scalar b) const {
        return Op::max(a, b);
    }

    SNorm* Maximum::constructor() {
        return new Maximum;
    }
}
