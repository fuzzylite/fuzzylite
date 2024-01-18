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

#ifndef FL_ANY_H
#define FL_ANY_H

#include "fl/hedge/Hedge.h"

namespace fl {

    /**
      The Any class is a special Hedge that always returns `1.0`. Its
      position with respect to the other hedges is last in the ordered set
      (Not, Seldom, Somewhat, Very, Extremely, Any). The Antecedent of a Rule
      considers Any to be a syntactically special hedge because it is not
      followed by a Term (e.g., `if Variable is any then...`). Amongst hedges,
      only Any has virtual methods to be overriden due to its particular case.

      @author Juan Rada-Vilela, Ph.D.
      @see Hedge
      @see HedgeFactory
      @since 4.0
     */
    class FL_API Any : public Hedge {
    public:
        Any();
        virtual ~Any() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Any)

        virtual std::string name() const FL_IOVERRIDE;


        virtual Complexity complexity() const FL_IOVERRIDE;

        /**
          Computes the hedge for the given value
          @param x is irrelevant
          @return `1.0`
         */
        virtual scalar hedge(scalar x) const FL_IOVERRIDE;
        virtual Any* clone() const FL_IOVERRIDE;

        static Hedge* constructor();
    };
}

#endif /* FL_ANY_H */
