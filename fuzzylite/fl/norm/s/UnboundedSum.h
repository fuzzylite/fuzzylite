/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.
 */

#ifndef FL_UNBOUNDEDSUM_H
#define FL_UNBOUNDEDSUM_H

#include "fl/norm/SNorm.h"

namespace fl {

    /**

      The UnboundedSum class is an SNorm that computes the sum of any two values.

      @author Juan Rada-Vilela, Ph.D.
      @see BoundedSum
      @see SNorm
      @see SNormFactory
      @see Norm
      @since 4.0
    
     */
    class FL_API UnboundedSum : public SNorm {
    public:
        std::string className() const FL_IOVERRIDE;
        /**
          Computes the bounded sum of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$\min(1, a+b)@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        UnboundedSum* clone() const FL_IOVERRIDE;

        static SNorm* constructor();
    };
}

#endif  /* FL_BOUNDEDSUM_H */
