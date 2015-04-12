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

#ifndef FL_TNORMFACTORY_H
#define FL_TNORMFACTORY_H

#include "fl/factory/ConstructionFactory.h"

#include "fl/norm/TNorm.h"

namespace fl {

    class FL_API TNormFactory : public ConstructionFactory<TNorm*> {
    public:
        TNormFactory();
        virtual ~TNormFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(TNormFactory)
    };
}
#endif  /* FL_TNORMFACTORY_H */

