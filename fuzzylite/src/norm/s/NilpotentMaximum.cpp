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


#include "fl/norm/s/NilpotentMaximum.h"


namespace fl {

    std::string NilpotentMaximum::className() const {
        return "NilpotentMaximum";
    }

    scalar NilpotentMaximum::compute(scalar a, scalar b) const {
        if (Op::isLt(a + b, 1.0)) {
            return std::max(a, b);
        }
        return 1.0;
    }

    NilpotentMaximum* NilpotentMaximum::clone() const {
        return new NilpotentMaximum(*this);
    }

    SNorm* NilpotentMaximum::constructor() {
        return new NilpotentMaximum;
    }


}

