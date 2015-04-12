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

#ifndef FL_FUNCTIONFACTORY_H
#define FL_FUNCTIONFACTORY_H

#include "fl/factory/CloningFactory.h"

#include "fl/term/Function.h"

namespace fl {

    class FunctionFactory : public CloningFactory<Function::Element*> {
    private:
        void registerOperators();
        void registerFunctions();
    public:
        FunctionFactory();
        virtual ~FunctionFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(FunctionFactory)

        virtual std::vector<std::string> availableOperators() const;
        virtual std::vector<std::string> availableFunctions() const;

    };

}

#endif  /* FL_FUNCTIONFACTORY_H */

