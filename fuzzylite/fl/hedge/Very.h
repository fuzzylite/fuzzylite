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

#ifndef FL_VERY_H
#define FL_VERY_H

#include "fl/hedge/Hedge.h"

namespace fl {

    /**
      The Very class is a Hedge located fourth in the ordered set
      (Not, Seldom, Somewhat, Very, Extremely, Any).

      @author Juan Rada-Vilela, Ph.D.
      @see Hedge
      @see HedgeFactory
      @since 4.0
     */
    class FL_API Very : public Hedge {
    public:
        std::string name() const FL_IOVERRIDE;

        Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the hedge for the membership function value @f$x@f$
          @param x is a membership function value
          @return @f$x^2@f$
         */
        scalar hedge(scalar x) const FL_IOVERRIDE;
        Very* clone() const FL_IOVERRIDE;

        static Hedge* constructor();
    };
}
#endif /* FL_VERY_H */
