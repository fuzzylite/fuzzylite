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

#ifndef FL_DEFUZZIFIERFACTORY_H
#define FL_DEFUZZIFIERFACTORY_H

#include "fuzzylite/defuzzifier/Defuzzifier.h"
#include "fuzzylite/defuzzifier/IntegralDefuzzifier.h"
#include "fuzzylite/defuzzifier/WeightedDefuzzifier.h"
#include "fuzzylite/factory/ConstructionFactory.h"

namespace fuzzylite {

    /**
      The DefuzzifierFactory class is a ConstructionFactory of Defuzzifier%s.

      @author Juan Rada-Vilela, Ph.D.
      @see Defuzzifier
      @see ConstructionFactory
      @see FactoryManager
      @since 4.0
     */
    class FL_API DefuzzifierFactory : public ConstructionFactory<Defuzzifier*> {
      public:
        explicit DefuzzifierFactory(const std::string& name = "Defuzzifier");
        virtual ~DefuzzifierFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(DefuzzifierFactory)

        /**
          Creates a Defuzzifier by executing the registered constructor
          @param key is the unique name by which constructors are registered
          @param resolution is the resolution of an IntegralDefuzzifier
          @param type is the type of a WeightedDefuzzifier
          @return a Defuzzifier by executing the registered constructor and
          setting its resolution or type accordingly
         */
        virtual Defuzzifier*
        constructDefuzzifier(const std::string& key, int resolution, WeightedDefuzzifier::Type type) const;

        /**
          Creates a Defuzzifier by executing the registered constructor
          @param defuzzifier is the unique name by which constructors are registered
          @param resolution is the resolution of an IntegralDefuzzifier
          @return a Defuzzifier by executing the registered constructor and
          setting its resolution
         */
        virtual Defuzzifier* constructIntegral(const std::string& defuzzifier, int resolution) const;

        /**
          Creates a Defuzzifier by executing the registered constructor
          @param defuzzifier is the unique name by which constructors are registered
          @param type is the type of a WeightedDefuzzifier
          @return a Defuzzifier by executing the registered constructor and
          setting its type
         */
        virtual Defuzzifier* constructWeighted(const std::string& defuzzifier, WeightedDefuzzifier::Type type) const;

        virtual DefuzzifierFactory* clone() const FL_IOVERRIDE;
    };
}

#endif /* DEFUZZIFIERFACTORY_H */
