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

#include "fuzzylite/defuzzifier/WeightedSum.h"

#include <map>

#include "fuzzylite/term/Aggregated.h"

namespace fuzzylite {

    WeightedSum::WeightedSum(Type type) : WeightedDefuzzifier(type) {}

    WeightedSum::WeightedSum(const std::string& type) : WeightedDefuzzifier(type) {}

    WeightedSum::~WeightedSum() {}

    std::string WeightedSum::className() const {
        return "WeightedSum";
    }

    scalar WeightedSum::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        const Aggregated* fuzzyOutput = dynamic_cast<const Aggregated*>(term);
        if (not fuzzyOutput) {
            std::ostringstream ss;
            ss << "[defuzzification error]"
               << "expected an Aggregated term instead of"
               << "<" << (term ? term->toString() : "null") << ">";
            throw Exception(ss.str(), FL_AT);
        }

        if (fuzzyOutput->isEmpty())
            return fl::nan;

        minimum = fuzzyOutput->getMinimum();
        maximum = fuzzyOutput->getMaximum();

        Type type = getType();
        if (type == Automatic)
            type = inferType(&(fuzzyOutput->terms().front()));

        scalar sum = 0.0;
        const std::size_t numberOfTerms = fuzzyOutput->numberOfTerms();
        if (type == TakagiSugeno) {
            // Provides Takagi-Sugeno and Inverse Tsukamoto of Functions
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
                z = activated.getTerm()->tsukamoto(w, minimum, maximum);
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
