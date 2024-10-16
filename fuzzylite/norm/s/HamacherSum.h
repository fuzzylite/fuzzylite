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

#ifndef FL_HAMACHERSUM_H
#define FL_HAMACHERSUM_H

#include "fuzzylite/norm/SNorm.h"

namespace fuzzylite {

    /**
      The HamacherSum class is an SNorm that computes the Hamacher sum of any
      two values.

      @image html S-HamacherSum.svg

      @author Juan Rada-Vilela, Ph.D.
      @see HamacherProduct
      @see SNorm
      @see SNormFactory
      @see Norm
      @since 4.0
     */
    class FL_API HamacherSum FL_IFINAL : public SNorm {
      public:
        std::string className() const FL_IOVERRIDE;

        /**
          Computes the Hamacher sum of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$a+b-(2\times a \times b)/(1-a\times b)@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        HamacherSum* clone() const FL_IOVERRIDE;

        static SNorm* constructor();
    };
}
#endif /* FL_HAMACHERSUM_H */
