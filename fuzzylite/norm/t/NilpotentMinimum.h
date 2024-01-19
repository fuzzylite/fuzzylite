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

#ifndef FL_NILPOTENTMINIMUM_H
#define FL_NILPOTENTMINIMUM_H

#include "fuzzylite/norm/TNorm.h"

namespace fuzzylite {

    /**
      The NilpotentMinimum class is a TNorm that computes the nilpotent minimum
      of any two values.

      @author Juan Rada-Vilela, Ph.D.
      @see NilpotentMaximum
      @see TNorm
      @see TNormFactory
      @see Norm
      @since 5.0
     */
    class FL_API NilpotentMinimum FL_IFINAL : public TNorm {
      public:
        std::string className() const FL_IOVERRIDE;

        /**
          Computes the nilpotent minimum of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$\begin{cases}
          \min(a,b) & \mbox{if $a+b>1$} \cr
          0 & \mbox{otherwise}
          \end{cases}@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        NilpotentMinimum* clone() const FL_IOVERRIDE;

        static TNorm* constructor();
    };
}
#endif /* FL_NILPOTENTMINIMUM_H */
