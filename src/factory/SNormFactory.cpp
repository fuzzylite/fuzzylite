/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
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
#include "fl/norm/s/UnboundedSum.h"

namespace fl {

    SNormFactory::SNormFactory() : ConstructionFactory<SNorm*>("SNorm") {
        registerConstructor("", fl::null);
        registerConstructor(AlgebraicSum().className(), &(AlgebraicSum::constructor));
        registerConstructor(BoundedSum().className(), &(BoundedSum::constructor));
        registerConstructor(DrasticSum().className(), &(DrasticSum::constructor));
        registerConstructor(EinsteinSum().className(), &(EinsteinSum::constructor));
        registerConstructor(HamacherSum().className(), &(HamacherSum::constructor));
        registerConstructor(Maximum().className(), &(Maximum::constructor));
        registerConstructor(NilpotentMaximum().className(), &(NilpotentMaximum::constructor));
        registerConstructor(NormalizedSum().className(), &(NormalizedSum::constructor));
        registerConstructor(UnboundedSum().className(), &(UnboundedSum::constructor));
    }

    SNormFactory::~SNormFactory() { }

}
