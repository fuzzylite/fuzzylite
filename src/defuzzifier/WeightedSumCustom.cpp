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

#include "fl/defuzzifier/WeightedSumCustom.h"

#include "fl/term/Aggregated.h"

#include <map>

namespace fl {

    WeightedSumCustom::WeightedSumCustom(Type type) : WeightedDefuzzifier(type) { }

    WeightedSumCustom::WeightedSumCustom(const std::string& type) : WeightedDefuzzifier(type) { }

    WeightedSumCustom::~WeightedSumCustom() { }

    std::string WeightedSumCustom::className() const {
        return "WeightedSumCustom";
    }

    Complexity WeightedSumCustom::complexity(const Term* term) const {
        Complexity result;
        result.comparison(4).function(1);
        const Aggregated* fuzzyOutput = dynamic_cast<const Aggregated*> (term);
        if (fuzzyOutput) {
            result += term->complexity().arithmetic(2).comparison(2)
                    .multiply(scalar(fuzzyOutput->numberOfTerms()));
        }
        return result;
    }

    scalar WeightedSumCustom::defuzzify(const Term* term,
            scalar minimum, scalar maximum) const {
        const Aggregated* fuzzyOutput = dynamic_cast<const Aggregated*> (term);
        if (not fuzzyOutput) {
            std::ostringstream ss;
            ss << "[defuzzification error]"
                    << "expected an Aggregated term instead of"
                    << "<" << (term ? term->toString() : "null") << ">";
            throw Exception(ss.str(), FL_AT);
        }

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
                wz = implication ? implication->compute(w, z) : (w * z);
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
                z = activated.getTerm()->tsukamoto(w, minimum, maximum);
                const TNorm* implication = activated.getImplication();
                wz = implication ? implication->compute(w, z) : (w * z);
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
