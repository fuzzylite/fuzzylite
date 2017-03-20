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

#ifndef FL_WEIGHTEDAVERAGECUSTOM_H
#define FL_WEIGHTEDAVERAGECUSTOM_H

#include "fl/defuzzifier/WeightedDefuzzifier.h"

namespace fl {
    class Activated;

    /**
      The (experimental) WeightedAverageCustom class is a WeightedDefuzzifier that computes the
      weighted average of a fuzzy set represented in an Aggregated Term utilizing
      the fuzzy operators for implication and aggregation to compute the weighted
      average.  This is an experimental approach to take advantage of customization
      thanks to the object-oriented design.

      @author Juan Rada-Vilela, Ph.D.
      @see WeightedAverage
      @see WeightedSum
      @see WeightedSumCustom
      @see WeightedDefuzzifier
      @see Defuzzifier
      @since 6.0
     */
    class FL_API WeightedAverageCustom : public WeightedDefuzzifier {
    public:
        explicit WeightedAverageCustom(Type type = Automatic);
        explicit WeightedAverageCustom(const std::string& type);
        virtual ~WeightedAverageCustom() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(WeightedAverageCustom)

        virtual std::string className() const FL_IOVERRIDE;

        virtual Complexity complexity(const Term* term) const FL_IOVERRIDE;

        /**
          Computes the weighted average of the given fuzzy set represented as
          an AggregatedTerm as @f$y = \dfrac{\sum_i w_iz_i}{\sum_i w_i} @f$,
          where @f$w_i@f$ is the activation degree of term @f$i@f$, and
          @f$z_i = \mu_i(w_i) @f$.

          If the implication and aggregation operators are set to fl::null (or
          set to AlgebraicProduct and UnboundedSum, respectively), then the
          operation of WeightedAverageCustom is the same as the WeightedAverage.
          Otherwise, the implication and aggregation operators are utilized to
          compute the multiplications and sums in @f$y@f$, respectively.

          @param term is the fuzzy set represented as an Aggregated Term
          @param minimum is the minimum value of the range (only used for Tsukamoto)
          @param maximum is the maximum value of the range (only used for Tsukamoto)
          @return the weighted average of the given fuzzy set
         */
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;
        virtual WeightedAverageCustom* clone() const FL_IOVERRIDE;

        static Defuzzifier* constructor();
    };
}

#endif  /* FL_WEIGHTEDAVERAGECUSTOM_H */

