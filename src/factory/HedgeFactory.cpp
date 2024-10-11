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

#include "fuzzylite/factory/HedgeFactory.h"

#include "fuzzylite/hedge/Any.h"
#include "fuzzylite/hedge/Extremely.h"
#include "fuzzylite/hedge/Not.h"
#include "fuzzylite/hedge/Seldom.h"
#include "fuzzylite/hedge/Somewhat.h"
#include "fuzzylite/hedge/Very.h"

namespace fuzzylite {

    HedgeFactory::HedgeFactory(const std::string& name) : ConstructionFactory(name) {
        ConstructionFactory::registerConstructor("", fl::null);
        ConstructionFactory::registerConstructor(Any().name(), &(Any::constructor));
        ConstructionFactory::registerConstructor(Extremely().name(), &(Extremely::constructor));
        ConstructionFactory::registerConstructor(Not().name(), &(Not::constructor));
        ConstructionFactory::registerConstructor(Seldom().name(), &(Seldom::constructor));
        ConstructionFactory::registerConstructor(Somewhat().name(), &(Somewhat::constructor));
        ConstructionFactory::registerConstructor(Very().name(), &(Very::constructor));
    }

    HedgeFactory::~HedgeFactory() {}

    HedgeFactory* HedgeFactory::clone() const {
        return new HedgeFactory(*this);
    }

}
