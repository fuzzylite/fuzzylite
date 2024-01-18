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

#ifndef FL_BOUNDEDDIFFERENCE_H
#define FL_BOUNDEDDIFFERENCE_H

#include "fl/norm/TNorm.h"

namespace fl {

    /**
      The BoundedDifference class is a TNorm that computes the bounded
      difference between any two values.

      @author Juan Rada-Vilela, Ph.D.
      @see BoundedSum
      @see TNorm
      @see TNormFactory
      @see Norm
      @since 4.0
     */
    class FL_API BoundedDifference FL_IFINAL : public TNorm {
    public:
        std::string className() const FL_IOVERRIDE;

        Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the bounded difference between two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$\max(0, a+b - 1)@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        BoundedDifference* clone() const FL_IOVERRIDE;

        static TNorm* constructor();
    };
}
#endif  /* FL_BOUNDEDDIFFERENCE_H */
