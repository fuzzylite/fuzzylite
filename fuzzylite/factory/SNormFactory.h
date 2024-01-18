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

#ifndef FL_SNORMFACTORY_H
#define FL_SNORMFACTORY_H

#include "fl/factory/ConstructionFactory.h"

#include "fl/norm/SNorm.h"

namespace fl {

    /**
      The SNormFactory class is a ConstructionFactory of SNorm%s.

      @author Juan Rada-Vilela, Ph.D.
      @see SNorm
      @see ConstructionFactory
      @see FactoryManager
      @since 4.0
     */
    class FL_API SNormFactory : public ConstructionFactory<SNorm*> {
    public:
        SNormFactory();
        virtual ~SNormFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(SNormFactory)
    };
}

#endif  /* FL_SNORMFACTORY_H */

