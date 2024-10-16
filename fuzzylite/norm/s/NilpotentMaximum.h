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

#ifndef FL_NILPOTENTMAXIMUM_H
#define FL_NILPOTENTMAXIMUM_H

#include "fuzzylite/norm/SNorm.h"

namespace fuzzylite {

    /**
      The NilpotentMaximum class is an SNorm that computes the nilpotent
      maximum of any two values.

      @image html S-NilpotentMaximum.svg

      @author Juan Rada-Vilela, Ph.D.
      @see NilpotentMinimum
      @see SNorm
      @see SNormFactory
      @see Norm
      @since 5.0
     */
    class FL_API NilpotentMaximum FL_IFINAL : public SNorm {
      public:
        std::string className() const FL_IOVERRIDE;

        /**
          Computes the nilpotent maximum of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$\begin{cases}
          \max(a,b) & \mbox{if $a+b<0$} \cr
          1 & \mbox{otherwise}
          \end{cases}@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        NilpotentMaximum* clone() const FL_IOVERRIDE;

        static SNorm* constructor();
    };
}

#endif /* FL_NILPOTENTMAXIMUM_H */
