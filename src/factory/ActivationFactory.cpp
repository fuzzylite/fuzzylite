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

#include "fl/factory/ActivationFactory.h"

#include "fl/activation/First.h"
#include "fl/activation/General.h"
#include "fl/activation/Highest.h"
#include "fl/activation/Last.h"
#include "fl/activation/Lowest.h"
#include "fl/activation/Proportional.h"
#include "fl/activation/Threshold.h"

namespace fl {

    ActivationFactory::ActivationFactory() : ConstructionFactory<Activation*>("Activation") {
        registerConstructor("", fl::null);
        registerConstructor(First().className(), &(First::constructor));
        registerConstructor(General().className(), &(General::constructor));
        registerConstructor(Highest().className(), &(Highest::constructor));
        registerConstructor(Last().className(), &(Last::constructor));
        registerConstructor(Lowest().className(), &(Lowest::constructor));
        registerConstructor(Proportional().className(), &(Proportional::constructor));
        registerConstructor(Threshold().className(), &(Threshold::constructor));
    }

    ActivationFactory::~ActivationFactory() { }

}
