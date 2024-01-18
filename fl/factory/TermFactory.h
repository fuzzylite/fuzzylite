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

#ifndef FL_TERMFACTORY_H
#define FL_TERMFACTORY_H


#include "fl/factory/ConstructionFactory.h"

#include "fl/term/Term.h"

namespace fl {

    /**
      The TermFactory class is a ConstructionFactory of Term%s.

      @author Juan Rada-Vilela, Ph.D.
      @see Term
      @see ConstructionFactory
      @see FactoryManager
      @since 4.0
     */
    class FL_API TermFactory : public ConstructionFactory<Term*> {
    public:
        TermFactory();
        virtual ~TermFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(TermFactory)
    };
}

#endif  /* FL_TERMFACTORY_H */

