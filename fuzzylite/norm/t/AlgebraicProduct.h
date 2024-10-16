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

#ifndef FL_ALGEBRAICPRODUCT_H
#define FL_ALGEBRAICPRODUCT_H

#include "fuzzylite/norm/TNorm.h"

namespace fuzzylite {

    /**
      The AlgebraicProduct class is a TNorm that computes the algebraic product
      of any two values.

      @image html T-AlgebraicProduct.svg

      @author Juan Rada-Vilela, Ph.D.
      @see AlgebraicSum
      @see TNorm
      @see TNormFactory
      @see Norm
      @since 4.0
     */
    class FL_API AlgebraicProduct FL_IFINAL : public TNorm {
      public:
        std::string className() const FL_IOVERRIDE;

        /**
          Computes the algebraic product of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$a\times b@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        AlgebraicProduct* clone() const FL_IOVERRIDE;

        static TNorm* constructor();
    };
}
#endif /* FL_ALGEBRAICPRODUCT_H */
