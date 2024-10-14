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

    scalar WeightedSum::takagiSugeno(const Aggregated* term) const {
        scalar sum = 0.0;
        std::vector<Activated> groupedTerms = term->groupedTerms();
        for (std::size_t i = 0; i < groupedTerms.size(); ++i) {
            const Activated& activated = groupedTerms.at(i);
            const scalar w = activated.getDegree();
            const scalar z = activated.getTerm()->membership(w);
            sum += w * z;
        }
        return sum;
    }

    scalar WeightedSum::tsukamoto(const Aggregated* term) const {
        scalar sum = 0.0;
        std::vector<Activated> groupedTerms = term->groupedTerms();
        for (std::size_t i = 0; i < groupedTerms.size(); ++i) {
            const Activated& activated = groupedTerms.at(i);
            const scalar w = activated.getDegree();
            const scalar z = activated.getTerm()->tsukamoto(w);
            sum += w * z;
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
