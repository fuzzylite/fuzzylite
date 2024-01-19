/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#ifndef FL_WEIGHTEDSUM_H
#define FL_WEIGHTEDSUM_H

#include "fuzzylite/defuzzifier/WeightedDefuzzifier.h"

namespace fuzzylite {

    /**
      The WeightedSum class is a WeightedDefuzzifier that computes the
      weighted sum of a fuzzy set represented in an Aggregated Term.

      @author Juan Rada-Vilela, Ph.D.
      @see WeightedSumCustom
      @see WeightedAverage
      @see WeightedAverageCustom
      @see WeightedDefuzzifier
      @see Defuzzifier
      @since 4.0
     */
    class FL_API WeightedSum : public WeightedDefuzzifier {
      public:
        explicit WeightedSum(Type type = Automatic);
        explicit WeightedSum(const std::string& type);
        virtual ~WeightedSum() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(WeightedSum)

        virtual std::string className() const FL_IOVERRIDE;

        /**
          Computes the weighted sum of the given fuzzy set represented as an
          Aggregated Term as @f$y = \sum_i{w_iz_i} @f$,
          where @f$w_i@f$ is the activation degree of term @f$i@f$, and @f$z_i
          = \mu_i(w_i) @f$.

          From version 6.0, the implication and aggregation operators are not
          utilized for defuzzification.

          @param term is the fuzzy set represented as an AggregatedTerm
          @param minimum is the minimum value of the range (only used for Tsukamoto)
          @param maximum is the maximum value of the range (only used for Tsukamoto)
          @return the weighted sum of the given fuzzy set
         */
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const FL_IOVERRIDE;
        virtual WeightedSum* clone() const FL_IOVERRIDE;

        static Defuzzifier* constructor();
    };
}

#endif /* FL_WEIGHTEDSUM_H */
