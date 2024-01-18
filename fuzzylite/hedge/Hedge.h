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

#ifndef FL_HEDGE_H
#define FL_HEDGE_H

#include "fl/fuzzylite.h"

#include "fl/Complexity.h"

#include <string>

namespace fl {

    /**
      The Hedge class is the abstract class for hedges. Hedges are utilized
      within the Antecedent and Consequent of a Rule in order to modify the
      membership function of a linguistic Term.

      @author Juan Rada-Vilela, Ph.D.
      @see Antecedent
      @see Consequent
      @see Rule
      @see HedgeFactory
      @since 4.0
     */
    class FL_API Hedge {
    public:

        Hedge() {
        }

        virtual ~Hedge() {
        }
        FL_DEFAULT_COPY_AND_MOVE(Hedge)

        /**
          Returns the name of the hedge
          @return the name of the hedge
         */
        virtual std::string name() const = 0;

        /**
          Computes the estimated complexity of applying the hedge
          @return the estimated complexity of applying the hedge
         */
        virtual Complexity complexity() const = 0;
        /**
          Computes the hedge for the membership function value @f$x@f$
          @param x is a membership function value
          @return the hedge of @f$x@f$
         */
        virtual scalar hedge(scalar x) const = 0;

        /**
          Creates a clone of the hedge
          @return a clone of the hedge.
         */
        virtual Hedge* clone() const = 0;

    };
}

#endif /* FL_HEDGE_H */
