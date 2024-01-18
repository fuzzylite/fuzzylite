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

#ifndef FL_WEIGHTEDSUMCUSTOM_H
#define FL_WEIGHTEDSUMCUSTOM_H


#include "fl/defuzzifier/WeightedDefuzzifier.h"

namespace fl {

    /**
      The (experimental) WeightedSumCustom class is a WeightedDefuzzifier that computes the
      weighted sum of a fuzzy set represented in an Aggregated Term utilizing
      the fuzzy operators for implication and aggregation to compute the weighted
      sum. This is an experimental approach to take advantage of customization
      thanks to the object-oriented design.

      @author Juan Rada-Vilela, Ph.D.
      @see WeightedSum
      @see WeightedAverage
      @see WeightedAverageCustom
      @see WeightedDefuzzifier
      @see Defuzzifier
      @since 6.0
      @experimental
     */
    class FL_API WeightedSumCustom : public WeightedDefuzzifier {
    public:
        explicit WeightedSumCustom(Type type = Automatic);
        explicit WeightedSumCustom(const std::string& type);
        virtual ~WeightedSumCustom() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(WeightedSumCustom)

        virtual std::string className() const FL_IOVERRIDE;

        virtual Complexity complexity(const Term* term) const FL_IOVERRIDE;

        /**
          Computes the weighted sum of the given fuzzy set represented in an
          Aggregated Term as @f$y = \sum_i{w_iz_i} @f$,
          where @f$w_i@f$ is the activation degree of term @f$i@f$, and @f$z_i
          = \mu_i(w_i) @f$.

          If the implication and aggregation operators are set to fl::null (or
          set to AlgebraicProduct and UnboundedSum, respectively), then the
          operation of WeightedAverageCustom is the same as the WeightedAverage.
          Otherwise, the implication and aggregation operators are utilized to
          compute the multiplications and sums in @f$y@f$, respectively.

          @param term is the fuzzy set represented as an AggregatedTerm
          @param minimum is the minimum value of the range (only used for Tsukamoto)
          @param maximum is the maximum value of the range (only used for Tsukamoto)
          @return the weighted sum of the given fuzzy set
         */
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;
        virtual WeightedSumCustom* clone() const FL_IOVERRIDE;

        static Defuzzifier* constructor();
    };
}

#endif  /* FL_WEIGHTEDSUMCUSTOM_H */

