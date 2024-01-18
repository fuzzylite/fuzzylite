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

#ifndef FL_BISECTOR_H
#define FL_BISECTOR_H

#include "fl/defuzzifier/IntegralDefuzzifier.h"

namespace fl {

    /**
      The Bisector class is an IntegralDefuzzifier that computes the bisector
      of a fuzzy set represented in a Term.

      @author Juan Rada-Vilela, Ph.D.
      @see Centroid
      @see IntegralDefuzzifier
      @see Defuzzifier
      @since 4.0
     */
    class FL_API Bisector : public IntegralDefuzzifier {
    public:
        explicit Bisector(int resolution = defaultResolution());
        virtual ~Bisector() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Bisector)

        virtual std::string className() const FL_IOVERRIDE;

        virtual Complexity complexity(const Term* term) const FL_IOVERRIDE;

        /**
          Computes the bisector of a fuzzy set. The defuzzification process
          integrates over the fuzzy set utilizing the boundaries given as
          parameters. The integration algorithm is the midpoint rectangle
          method (https://en.wikipedia.org/wiki/Rectangle_method).

          @param term is the fuzzy set
          @param minimum is the minimum value of the fuzzy set
          @param maximum is the maximum value of the fuzzy set
          @return the @f$x@f$-coordinate of the bisector of the fuzzy set
         */
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;
        virtual Bisector* clone() const FL_IOVERRIDE;

        static Defuzzifier* constructor();
    };
}

#endif  /* FL_BISECTOR_H */

