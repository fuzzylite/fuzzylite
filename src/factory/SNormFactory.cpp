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

#include "fuzzylite/factory/SNormFactory.h"

#include "fuzzylite/norm/s/AlgebraicSum.h"
#include "fuzzylite/norm/s/BoundedSum.h"
#include "fuzzylite/norm/s/DrasticSum.h"
#include "fuzzylite/norm/s/EinsteinSum.h"
#include "fuzzylite/norm/s/HamacherSum.h"
#include "fuzzylite/norm/s/Maximum.h"
#include "fuzzylite/norm/s/NilpotentMaximum.h"
#include "fuzzylite/norm/s/NormalizedSum.h"
#include "fuzzylite/norm/s/UnboundedSum.h"

namespace fuzzylite {

    SNormFactory::SNormFactory(const std::string& name) : ConstructionFactory(name) {
        ConstructionFactory::registerConstructor("", fl::null);
        ConstructionFactory::registerConstructor(AlgebraicSum().className(), &(AlgebraicSum::constructor));
        ConstructionFactory::registerConstructor(BoundedSum().className(), &(BoundedSum::constructor));
        ConstructionFactory::registerConstructor(DrasticSum().className(), &(DrasticSum::constructor));
        ConstructionFactory::registerConstructor(EinsteinSum().className(), &(EinsteinSum::constructor));
        ConstructionFactory::registerConstructor(HamacherSum().className(), &(HamacherSum::constructor));
        ConstructionFactory::registerConstructor(Maximum().className(), &(Maximum::constructor));
        ConstructionFactory::registerConstructor(NilpotentMaximum().className(), &(NilpotentMaximum::constructor));
        ConstructionFactory::registerConstructor(NormalizedSum().className(), &(NormalizedSum::constructor));
        ConstructionFactory::registerConstructor(UnboundedSum().className(), &(UnboundedSum::constructor));
    }

    SNormFactory::~SNormFactory() {}

    SNormFactory* SNormFactory::clone() const {
        return new SNormFactory(*this);
    }

}
