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

#ifndef FL_DEFUZZIFIER_H
#define FL_DEFUZZIFIER_H

#include "fl/fuzzylite.h"

#include "fl/Complexity.h"

#include <string>

namespace fl {
    class Term;

    /**
      The Defuzzifier class is the abstract class for defuzzifiers.

      @author Juan Rada-Vilela, Ph.D.
      @see IntegralDefuzzifier
      @see WeightedDefuzzifier
      @since 4.0
     */
    class FL_API Defuzzifier {
    public:

        Defuzzifier() {
        }

        virtual ~Defuzzifier() {
        }
        FL_DEFAULT_COPY_AND_MOVE(Defuzzifier)

        /**
          Returns the name of the class of the defuzzifier
          @return the name of the class of the defuzzifier
         */
        virtual std::string className() const = 0;
        /**
          Creates a clone of the defuzzifier
          @return a clone of the defuzzifier
         */
        virtual Defuzzifier* clone() const = 0;

        /**
          Computes the complexity of defuzzifying the given term
          @param term is the term to defuzzify
          @return the complexity of defuzzifying the given term
         */
        virtual Complexity complexity(const Term* term) const = 0;
        /**
          Defuzzifies the given fuzzy term utilizing the range `[minimum,maximum]`
          @param term is the term to defuzzify, typically an Aggregated term
          @param minimum is the minimum value of the range
          @param maximum is the maximum value of the range
          @return the defuzzified value of the given fuzzy term
         */
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const = 0;

    };
}

#endif /* FL_DEFUZZIFIER_H */
