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

#include "fl/factory/TNormFactory.h"

#include "fl/norm/t/AlgebraicProduct.h"
#include "fl/norm/t/BoundedDifference.h"
#include "fl/norm/t/DrasticProduct.h"
#include "fl/norm/t/EinsteinProduct.h"
#include "fl/norm/t/HamacherProduct.h"
#include "fl/norm/t/Minimum.h"
#include "fl/norm/t/NilpotentMinimum.h"

namespace fl {

    TNormFactory::TNormFactory() : ConstructionFactory<TNorm*>("TNorm") {
        registerConstructor("", fl::null);
        registerConstructor(AlgebraicProduct().className(), &(AlgebraicProduct::constructor));
        registerConstructor(BoundedDifference().className(), &(BoundedDifference::constructor));
        registerConstructor(DrasticProduct().className(), &(DrasticProduct::constructor));
        registerConstructor(EinsteinProduct().className(), &(EinsteinProduct::constructor));
        registerConstructor(HamacherProduct().className(), &(HamacherProduct::constructor));
        registerConstructor(Minimum().className(), &(Minimum::constructor));
        registerConstructor(NilpotentMinimum().className(), &(NilpotentMinimum::constructor));
    }

    TNormFactory::~TNormFactory() { }

}
