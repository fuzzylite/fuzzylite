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
 * Extremely.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/hedge/Extremely.h"
#include "fl/Operation.h"

namespace fl {

    std::string Extremely::name() const {
        return "extremely";
    }

    scalar Extremely::hedge(scalar x) const {
        return Op::isLE(x, 0.5)
                ? 2.0 * x * x
                : 1.0 - 2.0 * (1.0 - x) * (1.0 - x);
    }
    
    Hedge* Extremely::constructor(){
        return new Extremely;
    }
} 
