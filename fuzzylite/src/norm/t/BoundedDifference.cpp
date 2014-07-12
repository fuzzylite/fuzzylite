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
 * File:   BoundedDifference.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:54 AM
 */

#include "fl/norm/t/BoundedDifference.h"


namespace fl {

    std::string BoundedDifference::className() const {
        return "BoundedDifference";
    }

    scalar BoundedDifference::compute(scalar a, scalar b) const {
        return Op::max(scalar(0.0), a + b - 1.0);
    }

    BoundedDifference* BoundedDifference::clone() const {
        return new BoundedDifference(*this);
    }

    TNorm* BoundedDifference::constructor() {
        return new BoundedDifference;
    }

}