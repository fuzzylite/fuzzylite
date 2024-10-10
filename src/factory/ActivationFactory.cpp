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

#include "fuzzylite/factory/ActivationFactory.h"

#include "fuzzylite/activation/First.h"
#include "fuzzylite/activation/General.h"
#include "fuzzylite/activation/Highest.h"
#include "fuzzylite/activation/Last.h"
#include "fuzzylite/activation/Lowest.h"
#include "fuzzylite/activation/Proportional.h"
#include "fuzzylite/activation/Threshold.h"

namespace fuzzylite {

    ActivationFactory::ActivationFactory(const std::string& name) : ConstructionFactory(name) {
        ConstructionFactory::registerConstructor("", fl::null);
        ConstructionFactory::registerConstructor(First().className(), &(First::constructor));
        ConstructionFactory::registerConstructor(General().className(), &(General::constructor));
        ConstructionFactory::registerConstructor(Highest().className(), &(Highest::constructor));
        ConstructionFactory::registerConstructor(Last().className(), &(Last::constructor));
        ConstructionFactory::registerConstructor(Lowest().className(), &(Lowest::constructor));
        ConstructionFactory::registerConstructor(Proportional().className(), &(Proportional::constructor));
        ConstructionFactory::registerConstructor(Threshold().className(), &(Threshold::constructor));
    }

    ActivationFactory::~ActivationFactory() {}

    ActivationFactory* ActivationFactory::clone() const {
        return new ActivationFactory(*this);
    }

}
