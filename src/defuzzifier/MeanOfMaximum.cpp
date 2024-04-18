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

#include "fuzzylite/defuzzifier/MeanOfMaximum.h"

#include "fuzzylite/Exception.h"
#include "fuzzylite/term/Term.h"

namespace fuzzylite {

    MeanOfMaximum::MeanOfMaximum(int resolution) : IntegralDefuzzifier(resolution) {}

    MeanOfMaximum::~MeanOfMaximum() {}

    std::string MeanOfMaximum::className() const {
        return "MeanOfMaximum";
    }

    scalar MeanOfMaximum::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (not Op::isFinite(minimum + maximum))
            return fl::nan;
        const int resolution = getResolution();
        const scalar dx = (maximum - minimum) / resolution;
        scalar ymax = -fl::inf;
        std::vector<scalar> maxima;
        for (int i = 0; i < resolution; ++i) {
            const scalar x = minimum + (i + 0.5) * dx;
            const scalar y = term->membership(x);
            if (y > ymax) {
                maxima.clear();
                maxima.push_back(x);
                ymax = y;
            } else if (y == ymax)
                maxima.push_back(x);
        }
        const scalar mom = Op::mean(maxima);
        return mom;
    }

    MeanOfMaximum* MeanOfMaximum::clone() const {
        return new MeanOfMaximum(*this);
    }

    Defuzzifier* MeanOfMaximum::constructor() {
        return new MeanOfMaximum;
    }

}
