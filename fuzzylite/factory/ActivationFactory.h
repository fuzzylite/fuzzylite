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

#ifndef FL_ACTIVATIONFACTORY_H
#define FL_ACTIVATIONFACTORY_H

#include "fuzzylite/activation/Activation.h"
#include "fuzzylite/factory/ConstructionFactory.h"
#include "fuzzylite/fuzzylite.h"

namespace fuzzylite {

    /**
      The ActivationFactory class is a ConstructionFactory of Activation
      methods for RuleBlock%s.

      @author Juan Rada-Vilela, Ph.D.
      @see Activation
      @see RuleBlock
      @see ConstructionFactory
      @see FactoryManager
      @since 6.0
     */
    class FL_API ActivationFactory : public ConstructionFactory<Activation*> {
      public:
        ActivationFactory();
        virtual ~ActivationFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(ActivationFactory)
    };
}

#endif /* FL_ACTIVATIONFACTORY_H */
