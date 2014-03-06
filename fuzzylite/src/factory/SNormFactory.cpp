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
 * File:   SNormFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:20 PM
 */

#include "fl/factory/SNormFactory.h"

#include "fl/norm/s/AlgebraicSum.h"
#include "fl/norm/s/BoundedSum.h"
#include "fl/norm/s/DrasticSum.h"
#include "fl/norm/s/EinsteinSum.h"
#include "fl/norm/s/HamacherSum.h"
#include "fl/norm/s/Maximum.h"
#include "fl/norm/s/NilpotentMaximum.h"
#include "fl/norm/s/NormalizedSum.h"

namespace fl {

    SNormFactory::SNormFactory() {
        registerClass(AlgebraicSum().className(), &(AlgebraicSum::constructor));
        registerClass(BoundedSum().className(), &(BoundedSum::constructor));
        registerClass(DrasticSum().className(), &(DrasticSum::constructor));
        registerClass(EinsteinSum().className(), &(EinsteinSum::constructor));
        registerClass(HamacherSum().className(), &(HamacherSum::constructor));
        registerClass(Maximum().className(), &(Maximum::constructor));
        registerClass(NilpotentMaximum().className(), &(NilpotentMaximum::constructor));
        registerClass(NormalizedSum().className(), &(NormalizedSum::constructor));
    }

    SNormFactory::~SNormFactory() {
    }

}
