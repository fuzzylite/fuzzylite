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

#ifndef FL_FUNCTIONFACTORY_H
#define FL_FUNCTIONFACTORY_H

#include "fl/factory/CloningFactory.h"

#include "fl/term/Function.h"

namespace fl {

    /**
      The FunctionFactory class is a CloningFactory of operators and functions
      utilized by the Function term.

      @author Juan Rada-Vilela, Ph.D.
      @see Function
      @see Element
      @see CloningFactory
      @see FactoryManager
      @since 5.0
     */
    class FL_API FunctionFactory : public CloningFactory<Function::Element*> {
    private:
        void registerOperators();
        void registerFunctions();
    public:
        FunctionFactory();
        virtual ~FunctionFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FunctionFactory)

        /**
          Returns a vector of the operators available
          @return a vector of the operators available
         */
        virtual std::vector<std::string> availableOperators() const;
        /**
          Returns a vector of the functions available
          @return a vector of the functions available
         */
        virtual std::vector<std::string> availableFunctions() const;

    };
}

#endif  /* FL_FUNCTIONFACTORY_H */

