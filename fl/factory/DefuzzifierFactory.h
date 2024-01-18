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

#ifndef FL_DEFUZZIFIERFACTORY_H
#define FL_DEFUZZIFIERFACTORY_H

#include "fl/factory/ConstructionFactory.h"

#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/defuzzifier/IntegralDefuzzifier.h"
#include "fl/defuzzifier/WeightedDefuzzifier.h"

namespace fl {

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
        DefuzzifierFactory();
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
        virtual Defuzzifier* constructDefuzzifier(const std::string& key,
                int resolution, WeightedDefuzzifier::Type type) const;

        /**
          Creates a Defuzzifier by executing the registered constructor
          @param key is the unique name by which constructors are registered
          @param resolution is the resolution of an IntegralDefuzzifier
          @return a Defuzzifier by executing the registered constructor and
          setting its resolution
         */
        virtual Defuzzifier* constructDefuzzifier(const std::string& key, int resolution) const;

        /**
          Creates a Defuzzifier by executing the registered constructor
          @param key is the unique name by which constructors are registered
          @param type is the type of a WeightedDefuzzifier
          @return a Defuzzifier by executing the registered constructor and
          setting its type
         */
        virtual Defuzzifier* constructDefuzzifier(const std::string& key, WeightedDefuzzifier::Type type);
    };
}

#endif  /* DEFUZZIFIERFACTORY_H */

