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

#ifndef FL_EINSTEINSUM_H
#define FL_EINSTEINSUM_H

#include "fl/norm/SNorm.h"

namespace fl {

    /**
      The EinsteinSum class is an SNorm that computes the einstein sum of any
      two values.

      @author Juan Rada-Vilela, Ph.D.
      @see EinsteinProduct
      @see SNorm
      @see SNormFactory
      @see Norm
      @since 4.0
     */
    class FL_API EinsteinSum FL_IFINAL : public SNorm {
    public:
        std::string className() const FL_IOVERRIDE;

        Complexity complexity() const FL_IOVERRIDE;
        /**
          Computes the Einstein sum of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$a+b/(1+a \times b)@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        EinsteinSum* clone() const FL_IOVERRIDE;

        static SNorm* constructor();
    };
}
#endif  /* FL_EINSTEINSUM_H */
