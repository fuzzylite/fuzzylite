/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/factory/HedgeFactory.h"

#include "fl/hedge/Any.h"
#include "fl/hedge/Extremely.h"
#include "fl/hedge/Not.h"
#include "fl/hedge/Seldom.h"
#include "fl/hedge/Somewhat.h"
#include "fl/hedge/Very.h"


namespace fl {

    HedgeFactory::HedgeFactory() : ConstructionFactory<Hedge*>("Hedge") {
        registerConstructor("", fl::null);
        registerConstructor(Any().name(), &(Any::constructor));
        registerConstructor(Extremely().name(), &(Extremely::constructor));
        registerConstructor(Not().name(), &(Not::constructor));
        registerConstructor(Seldom().name(), &(Seldom::constructor));
        registerConstructor(Somewhat().name(), &(Somewhat::constructor));
        registerConstructor(Very().name(), &(Very::constructor));
    }

    HedgeFactory::~HedgeFactory() {

    }

}
