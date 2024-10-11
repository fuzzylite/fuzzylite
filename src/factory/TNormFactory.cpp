/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/factory/TNormFactory.h"

#include "fuzzylite/norm/t/AlgebraicProduct.h"
#include "fuzzylite/norm/t/BoundedDifference.h"
#include "fuzzylite/norm/t/DrasticProduct.h"
#include "fuzzylite/norm/t/EinsteinProduct.h"
#include "fuzzylite/norm/t/HamacherProduct.h"
#include "fuzzylite/norm/t/Minimum.h"
#include "fuzzylite/norm/t/NilpotentMinimum.h"

namespace fuzzylite {

    TNormFactory::TNormFactory(const std::string& name) : ConstructionFactory(name) {
        ConstructionFactory::registerConstructor("", fl::null);
        ConstructionFactory::registerConstructor(AlgebraicProduct().className(), &(AlgebraicProduct::constructor));
        ConstructionFactory::registerConstructor(BoundedDifference().className(), &(BoundedDifference::constructor));
        ConstructionFactory::registerConstructor(DrasticProduct().className(), &(DrasticProduct::constructor));
        ConstructionFactory::registerConstructor(EinsteinProduct().className(), &(EinsteinProduct::constructor));
        ConstructionFactory::registerConstructor(HamacherProduct().className(), &(HamacherProduct::constructor));
        ConstructionFactory::registerConstructor(Minimum().className(), &(Minimum::constructor));
        ConstructionFactory::registerConstructor(NilpotentMinimum().className(), &(NilpotentMinimum::constructor));
    }

    TNormFactory::~TNormFactory() {}

    TNormFactory* TNormFactory::clone() const {
        return new TNormFactory(*this);
    }

}
