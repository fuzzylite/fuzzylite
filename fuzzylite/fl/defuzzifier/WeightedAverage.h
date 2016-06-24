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

#ifndef FL_WEIGHTEDAVERAGE_H
#define FL_WEIGHTEDAVERAGE_H

#include "fl/defuzzifier/WeightedDefuzzifier.h"

namespace fl {
    class Activated;

    /**

      The WeightedAverage class is a WeightedDefuzzifier that computes the
      weighted average of a fuzzy set represented in an Aggregated Term.

      @author Juan Rada-Vilela, Ph.D.
      @see WeightedAverageCustom
      @see WeightedSum
      @see WeightedSumCustom
      @see WeightedDefuzzifier
      @see Defuzzifier
      @since 4.0

     */
    class FL_API WeightedAverage : public WeightedDefuzzifier {
    public:
        explicit WeightedAverage(Type type = Automatic);
        explicit WeightedAverage(const std::string& type);
        virtual ~WeightedAverage() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(WeightedAverage)

        virtual std::string className() const FL_IOVERRIDE;
        /**
          Computes the weighted average of a fuzzy set represented in
          an AggregatedTerm as @f$y = \dfrac{\sum_i w_iz_i}{\sum_i w_i} @f$,
          where @f$w_i@f$ is the activation degree of term @f$i@f$, and
          @f$z_i = \mu_i(w_i) @f$.

          From version 6.0, the implication and aggregation operators are not
          utilized for defuzzification. Also, for better performance, the term
          is assumed to be Aggregated without type checking, for which unexpected
          operation may occur if the term is not an instance of Aggregated.

          @param term is the fuzzy set represented as an Aggregated Term
          @param minimum is the minimum value of the range (only used for Tsukamoto)
          @param maximum is the maximum value of the range (only used for Tsukamoto)
          @return
         */
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const FL_IOVERRIDE;
        virtual WeightedAverage* clone() const FL_IOVERRIDE;

        static Defuzzifier* constructor();
    };
}

#endif  /* FL_WEIGHTEDAVERAGE_H */

