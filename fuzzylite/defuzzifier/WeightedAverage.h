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

#ifndef FL_WEIGHTEDAVERAGE_H
#define FL_WEIGHTEDAVERAGE_H

#include "fuzzylite/defuzzifier/WeightedDefuzzifier.h"

namespace fuzzylite {
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
        virtual scalar takagiSugeno(const Aggregated* term) const FL_IOVERRIDE;
        virtual scalar tsukamoto(const Aggregated* term) const FL_IOVERRIDE;

        virtual WeightedAverage* clone() const FL_IOVERRIDE;

        static Defuzzifier* constructor();
    };
}

#endif /* FL_WEIGHTEDAVERAGE_H */
