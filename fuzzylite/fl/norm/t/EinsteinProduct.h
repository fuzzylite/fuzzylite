/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.
 */

#ifndef FL_EINSTEINPRODUCT_H
#define FL_EINSTEINPRODUCT_H

#include "fl/norm/TNorm.h"

namespace fl {

    /**
    
      The EinsteinProduct class is a TNorm that computes the einstein product
      of any two values.
    
      @author Juan Rada-Vilela, Ph.D.
      @see EinsteinSum
      @see TNorm
      @see TNormFactory
      @see Norm
      @since 4.0
    
     */
    class FL_API EinsteinProduct : public TNorm {
    public:
        std::string className() const FL_IOVERRIDE;
        /**
          Computes the einstein product of two membership function values
          @param a is a membership function value
          @param b is a membership function value
          @return @f$(a\times b)/(2-(a+b-a\times b))@f$
         */
        scalar compute(scalar a, scalar b) const FL_IOVERRIDE;
        EinsteinProduct* clone() const FL_IOVERRIDE;

        static TNorm* constructor();
    };


}

#endif  /* FL_EINSTEINPRODUCT_H */
