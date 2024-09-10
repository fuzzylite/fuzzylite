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

#include "fuzzylite/defuzzifier/Bisector.h"

#include "fuzzylite/term/Term.h"

namespace fuzzylite {

    Bisector::Bisector(int resolution) : IntegralDefuzzifier(resolution) {}

    Bisector::~Bisector() {}

    std::string Bisector::className() const {
        return "Bisector";
    }

    scalar Bisector::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (not Op::isFinite(minimum + maximum))
            return fl::nan;

        const std::vector<scalar> x = Op::midpoints(minimum, maximum, getResolution());
        std::vector<scalar> ysum;
        ysum.reserve(getResolution());
        scalar sum = 0.0;

        // Compute area
        for (std::size_t i = 0; i < x.size(); ++i) {
            const scalar yi = term->membership(x.at(i));
            if (Op::isFinite(yi))
                sum += yi;
            ysum.push_back(sum);
        }

        // Normalize area
        minimum = fl::inf;
        for (std::size_t i = 0; i < x.size(); ++i) {
            ysum.at(i) = fabs(ysum.at(i) / sum - 0.5);
            if (ysum.at(i) < minimum)
                minimum = ysum.at(i);
        }

        // All minima x are bisectors
        std::vector<scalar> bisectors;
        for (std::size_t i = 0; i < x.size(); ++i)
            if (minimum == ysum.at(i))
                bisectors.push_back(x.at(i));
        const scalar bisector = Op::mean(bisectors);
        return bisector;
    }

    Bisector* Bisector::clone() const {
        return new Bisector(*this);
    }

    Defuzzifier* Bisector::constructor() {
        return new Bisector;
    }

}
