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

#include "fl/defuzzifier/WeightedSum.h"

#include "fl/term/Aggregated.h"
#include "fl/term/Activated.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"

#include <map>

namespace fl {

    WeightedSum::WeightedSum(Type type) : WeightedDefuzzifier(type) {
    }

    WeightedSum::WeightedSum(const std::string& type) : WeightedDefuzzifier(type) {

    }

    WeightedSum::~WeightedSum() {
    }

    std::string WeightedSum::className() const {
        return "WeightedSum";
    }

    Complexity WeightedSum::complexity(const Term* term) const {
        Complexity result;
        result.comparison(4).function(1);
        const Aggregated* fuzzyOutput = dynamic_cast<const Aggregated*> (term);
        if (fuzzyOutput) {
            result += term->complexity().arithmetic(2).multiply(fuzzyOutput->numberOfTerms());
        }
        return result;
    }

    scalar WeightedSum::defuzzify(const Term* term,
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

        scalar sum = 0.0;
        const std::size_t numberOfTerms = fuzzyOutput->numberOfTerms();
        if (type == TakagiSugeno) {
            //Provides Takagi-Sugeno and Inverse Tsukamoto of Functions
            scalar w, z;
            for (std::size_t i = 0; i < numberOfTerms; ++i) {
                const Activated& activated = fuzzyOutput->getTerm(i);
                w = activated.getDegree();
                z = activated.getTerm()->membership(w);
                sum += w * z;
            }
        } else {
            scalar w, z;
            for (std::size_t i = 0; i < numberOfTerms; ++i) {
                const Activated& activated = fuzzyOutput->getTerm(i);
                w = activated.getDegree();
                z = tsukamoto(activated.getTerm(), w, minimum, maximum);
                sum += w * z;
            }
        }
        return sum;
    }

    WeightedSum* WeightedSum::clone() const {
        return new WeightedSum(*this);
    }

    Defuzzifier* WeightedSum::constructor() {
        return new WeightedSum;
    }

}
