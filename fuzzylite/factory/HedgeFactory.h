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

#ifndef FL_HEDGEFACTORY_H
#define FL_HEDGEFACTORY_H

#include "fuzzylite/factory/ConstructionFactory.h"
#include "fuzzylite/hedge/Hedge.h"

namespace fuzzylite {

    /**
      The HedgeFactory class is a ConstructionFactory of Hedge%s.

      @author Juan Rada-Vilela, Ph.D.
      @see Hedge
      @see ConstructionFactory
      @see FactoryManager
      @since 4.0
     */
    class FL_API HedgeFactory : public ConstructionFactory<Hedge*> {
      public:
        explicit HedgeFactory(const std::string& name = "Hedge");
        virtual ~HedgeFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(HedgeFactory)

        virtual HedgeFactory* clone() const FL_IOVERRIDE;
    };
}

#endif /* FL_HEDGEFACTORY_H */
