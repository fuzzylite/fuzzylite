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

#ifndef FL_ACTIVATIONFACTORY_H
#define	FL_ACTIVATIONFACTORY_H

#include "fl/fuzzylite.h"

#include "fl/factory/ConstructionFactory.h"
#include "fl/activation/Activation.h"

namespace fl {

    class FL_API ActivationFactory : public ConstructionFactory<Activation*> {
    public:
        ActivationFactory();
        virtual ~ActivationFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(ActivationFactory)
    };
}

#endif	/* FL_ACTIVATIONFACTORY_H */

