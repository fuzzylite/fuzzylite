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

#include "fl/defuzzifier/WeightedSumCustom.h"

#include "fl/term/Aggregated.h"
#include "fl/term/Activated.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

#include <map>

namespace fl {

    WeightedSumCustom::WeightedSumCustom(Type type) : WeightedDefuzzifier(type) {
    }

    WeightedSumCustom::WeightedSumCustom(const std::string& type) : WeightedDefuzzifier(type) {

    }

    WeightedSumCustom::~WeightedSumCustom() {
    }

    std::string WeightedSumCustom::className() const {
        return "WeightedSumCustom";
    }

    scalar WeightedSumCustom::defuzzify(const Term* term,
            scalar minimum, scalar maximum) const {
        const Aggregated* fuzzyOutput = static_cast<const Aggregated*> (term);
        if (fuzzyOutput->isEmpty()) return fl::nan;

        minimum = fuzzyOutput->getMinimum();
        maximum = fuzzyOutput->getMaximum();

        Type type = getType();
        if (type == Automatic) {
            type = inferType(&(fuzzyOutput->terms().front()));
        }

        SNorm* aggregation = fuzzyOutput->getAggregation();

        scalar sum = 0.0;
        const std::size_t numberOfTerms = fuzzyOutput->numberOfTerms();
        if (type == TakagiSugeno) {
            //Provides Takagi-Sugeno and Inverse Tsukamoto of Functions
            scalar w, z, wz;
            for (std::size_t i = 0; i < numberOfTerms; ++i) {
                const Activated& activated = fuzzyOutput->getTerm(i);
                w = activated.getDegree();
                z = activated.getTerm()->membership(w);
                const TNorm* implication = activated.getImplication();
                wz = implication ? implication->compute(w, z) : w*z;
                if (aggregation) {
                    sum = aggregation->compute(sum, wz);
                } else {
                    sum += wz;
                }
            }
        } else {
            scalar w, z, wz;
            for (std::size_t i = 0; i < numberOfTerms; ++i) {
                const Activated& activated = fuzzyOutput->getTerm(i);
                w = activated.getDegree();
                z = tsukamoto(activated.getTerm(), w, minimum, maximum);
                const TNorm* implication = activated.getImplication();
                wz = implication ? implication->compute(w, z) : w*z;
                if (aggregation) {
                    sum = aggregation->compute(sum, wz);
                } else {
                    sum += wz;
                }
            }
        }
        return sum;
    }

    WeightedSumCustom* WeightedSumCustom::clone() const {
        return new WeightedSumCustom(*this);
    }

    Defuzzifier* WeightedSumCustom::constructor() {
        return new WeightedSumCustom;
    }

}
