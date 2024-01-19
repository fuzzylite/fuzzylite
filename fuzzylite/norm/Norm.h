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

#ifndef FL_NORM_H
#define FL_NORM_H

#include <string>

#include "fuzzylite/fuzzylite.h"

namespace fuzzylite {

    /**
      The Norm class is the abstract class for norms.

      @author Juan Rada-Vilela, Ph.D.
      @see TNorm
      @see SNorm
      @see TNormFactory
      @see SNormFactory
      @since 4.0
     */
    class FL_API Norm {
      public:
        Norm() {}

        virtual ~Norm() {}

        FL_DEFAULT_COPY_AND_MOVE(Norm)
        /**
          Returns the name of the class of the norm
          @return the name of the class of the norm
         */
        virtual std::string className() const = 0;

        /**
          Computes the norm for @f$a@f$ and @f$b@f$
          @param a is a membership function value
          @param b is a membership function value
          @return the norm between @f$a@f$ and @f$b@f$
         */
        virtual scalar compute(scalar a, scalar b) const = 0;

        /**
          Creates a clone of the norm
          @return a clone of the norm
         */
        virtual Norm* clone() const = 0;
    };
}
#endif /* FL_NORM_H */
