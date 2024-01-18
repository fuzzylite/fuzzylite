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

#ifndef FL_SOMEWHAT_H
#define FL_SOMEWHAT_H

#include "fl/hedge/Hedge.h"

namespace fl {

    /**
      The Somewhat class is a Hedge located third in the ordered set
      (Not, Seldom, Somewhat, Very, Extremely, Any).

      @author Juan Rada-Vilela, Ph.D.
      @see Hedge
      @see HedgeFactory
      @since 4.0
     */
    class FL_API Somewhat FL_IFINAL : public Hedge {
    public:
        std::string name() const FL_IOVERRIDE;


        Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the hedge for the membership function value @f$x@f$
          @param x is a membership function value
          @return @f$\sqrt{x}@f$
         */
        scalar hedge(scalar x) const FL_IOVERRIDE;
        Somewhat* clone() const FL_IOVERRIDE;

        static Hedge* constructor();
    };
}

#endif /* FL_SOMEWHAT_H */
