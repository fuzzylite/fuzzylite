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
 * File:   TNormFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013) return  11:20 PM
 */

#include "fl/factory/TNormFactory.h"

#include "fl/norm/t/AlgebraicProduct.h"
#include "fl/norm/t/BoundedDifference.h"
#include "fl/norm/t/DrasticProduct.h"
#include "fl/norm/t/EinsteinProduct.h"
#include "fl/norm/t/HamacherProduct.h"
#include "fl/norm/t/Minimum.h"
#include "fl/norm/t/NilpotentMinimum.h"

namespace fl {

    TNormFactory::TNormFactory() {
        registerClass(AlgebraicProduct().className(), &(AlgebraicProduct::constructor));
        registerClass(BoundedDifference().className(), &(BoundedDifference::constructor));
        registerClass(DrasticProduct().className(), &(DrasticProduct::constructor));
        registerClass(EinsteinProduct().className(), &(EinsteinProduct::constructor));
        registerClass(HamacherProduct().className(), &(HamacherProduct::constructor));
        registerClass(Minimum().className(), &(Minimum::constructor));
        registerClass(NilpotentMinimum().className(), &(NilpotentMinimum::constructor));
    }

    TNormFactory::~TNormFactory() {
    }

}
