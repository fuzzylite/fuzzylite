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

#include "fuzzylite/defuzzifier/WeightedAverage.h"

#include <map>

#include "fuzzylite/term/Aggregated.h"

namespace fuzzylite {

    WeightedAverage::WeightedAverage(Type type) : WeightedDefuzzifier(type) {}

    WeightedAverage::WeightedAverage(const std::string& type) : WeightedDefuzzifier(type) {}

    WeightedAverage::~WeightedAverage() {}

    std::string WeightedAverage::className() const {
        return "WeightedAverage";
    }

    scalar WeightedAverage::takagiSugeno(const Aggregated* term) const {
        scalar sum = 0.0;
        scalar weights = 0.0;
        std::vector<Activated> groupedTerms(term->groupedTerms());
        for (std::size_t i = 0; i < groupedTerms.size(); ++i) {
            const Activated& activated = groupedTerms.at(i);
            const scalar w = activated.getDegree();
            const scalar z = activated.getTerm()->membership(w);
            sum += w * z;
            weights += w;
        }
        return sum / weights;
    }

    scalar WeightedAverage::tsukamoto(const Aggregated* term) const {
        scalar sum = 0.0;
        scalar weights = 0.0;
        std::vector<Activated> groupedTerms(term->groupedTerms());
        for (std::size_t i = 0; i < groupedTerms.size(); ++i) {
            const Activated& activated = groupedTerms.at(i);
            const scalar w = activated.getDegree();
            const scalar z = activated.getTerm()->tsukamoto(w);
            sum += w * z;
            weights += w;
        }
        return sum / weights;
    }

    WeightedAverage* WeightedAverage::clone() const {
        return new WeightedAverage(*this);
    }

    Defuzzifier* WeightedAverage::constructor() {
        return new WeightedAverage;
    }

}
