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

#ifndef FL_DRASTICPRODUCT_H
#define FL_DRASTICPRODUCT_H

#include "fl/norm/TNorm.h"

namespace fl {

    /**
      The DrasticProduct class is a TNorm that computes the drastic product of
      any two values.

      @author Juan Rada-Vilela, Ph.D.
      @see DrasticSum
      @see TNorm
      @see TNormFactory
      @see Norm
      @since 4.0
     */
    class FL_API DrasticProduct FL_IFINAL : public TNorm {
    public:
        std::string className() const FL_IOVERRIDE;

        Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the drastic product of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$\begin{cases}
          \min(a,b) & \mbox{if $\max(a,b)=1$} \cr
          0 & \mbox{otherwise}
          \end{cases}@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        DrasticProduct* clone() const FL_IOVERRIDE;

        static TNorm* constructor();
    };
}
#endif  /* FL_DRASTICPRODUCT_H */
