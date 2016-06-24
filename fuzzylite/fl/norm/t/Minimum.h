/*
 Copyright (C) 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite(R).

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite(R) is a registered trademark of FuzzyLite Limited.
 */

#ifndef FL_MINIMUM_H
#define FL_MINIMUM_H

#include "fl/norm/TNorm.h"

namespace fl {

    /**

      The Minimum class is a TNorm that computes the minimum of any two values.

      @author Juan Rada-Vilela, Ph.D.
      @see Maximum
      @see TNorm
      @see TNormFactory
      @see Norm
      @since 4.0

     */
    class FL_API Minimum : public TNorm {
    public:
        std::string className() const FL_IOVERRIDE;
        /**
          Computes the minimum of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$\min(a,b)@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        Minimum* clone() const FL_IOVERRIDE;

        static TNorm* constructor();
    };
}
#endif  /* FL_MINIMUM_H */
