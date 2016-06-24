/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/defuzzifier/WeightedAverageCustom.h"

#include "fl/term/Aggregated.h"
#include "fl/term/Activated.h"
#include "fl/norm/Norm.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

#include <map>

namespace fl {

    WeightedAverageCustom::WeightedAverageCustom(Type type) : WeightedDefuzzifier(type) {
    }

    WeightedAverageCustom::WeightedAverageCustom(const std::string& type) : WeightedDefuzzifier(type) {
    }

    WeightedAverageCustom::~WeightedAverageCustom() {
    }

    std::string WeightedAverageCustom::className() const {
        return "WeightedAverageCustom";
    }

    scalar WeightedAverageCustom::defuzzify(const Term* term,
            scalar minimum, scalar maximum) const {
        const Aggregated* fuzzyOutput = static_cast<const Aggregated*> (term);

        minimum = fuzzyOutput->getMinimum();
        maximum = fuzzyOutput->getMaximum();

        SNorm* aggregation = fuzzyOutput->getAggregation();

        const std::size_t numberOfTerms = fuzzyOutput->numberOfTerms();
        Type type = getType();
        if (type == Automatic and numberOfTerms > 0) {
            type = inferType(&(fuzzyOutput->terms().front()));
        }

        scalar sum = 0.0;
        scalar weights = 0.0;
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
                    weights = aggregation->compute(weights, w);
                } else {
                    sum += wz;
                    weights += w;
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
                    weights = aggregation->compute(weights, w);
                } else {
                    sum += wz;
                    weights += w;
                }
            }
        }
        return sum / weights;
    }

    WeightedAverageCustom* WeightedAverageCustom::clone() const {
        return new WeightedAverageCustom(*this);
    }

    Defuzzifier* WeightedAverageCustom::constructor() {
        return new WeightedAverageCustom;
    }

}
