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

#include "fuzzylite/defuzzifier/SmallestOfMaximum.h"

#include "fuzzylite/Exception.h"
#include "fuzzylite/term/Term.h"

namespace fuzzylite {

    SmallestOfMaximum::SmallestOfMaximum(int resolution) : IntegralDefuzzifier(resolution) {}

    SmallestOfMaximum::~SmallestOfMaximum() {}

    std::string SmallestOfMaximum::className() const {
        return "SmallestOfMaximum";
    }

    scalar SmallestOfMaximum::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (not Op::isFinite(minimum + maximum))
            return fl::nan;
        const int resolution = getResolution();
        const scalar dx = (maximum - minimum) / resolution;
        scalar ymax = -fl::inf;
        scalar som = fl::nan;
        for (int i = 0; i < resolution; ++i) {
            const scalar x = minimum + (i + 0.5) * dx;
            const scalar y = term->membership(x);
            if (y > ymax) {
                som = x;
                ymax = y;
            }
        }
        return som;
    }

    SmallestOfMaximum* SmallestOfMaximum::clone() const {
        return new SmallestOfMaximum(*this);
    }

    Defuzzifier* SmallestOfMaximum::constructor() {
        return new SmallestOfMaximum;
    }

}
