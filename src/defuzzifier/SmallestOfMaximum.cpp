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

#include "fl/defuzzifier/SmallestOfMaximum.h"

#include "fl/Exception.h"
#include "fl/term/Term.h"

namespace fl {

    SmallestOfMaximum::SmallestOfMaximum(int resolution)
    : IntegralDefuzzifier(resolution) { }

    SmallestOfMaximum::~SmallestOfMaximum() { }

    std::string SmallestOfMaximum::className() const {
        return "SmallestOfMaximum";
    }

    Complexity SmallestOfMaximum::complexity(const Term* term) const {
        return Complexity().comparison(1).arithmetic(1 + 2) +
                term->complexity().comparison(1).arithmetic(3).multiply(getResolution());
    }

    scalar SmallestOfMaximum::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (not Op::isFinite(minimum + maximum)) return fl::nan;

        const int resolution = getResolution();
        const scalar dx = (maximum - minimum) / resolution;
        scalar x, y;
        scalar ymax = -1.0, xsmallest = minimum;
        for (int i = 0; i < resolution; ++i) {
            x = minimum + (i + 0.5) * dx;
            y = term->membership(x);

            if (Op::isGt(y, ymax)) {
                xsmallest = x;
                ymax = y;
            }
        }
        return xsmallest;
    }

    SmallestOfMaximum* SmallestOfMaximum::clone() const {
        return new SmallestOfMaximum(*this);
    }

    Defuzzifier* SmallestOfMaximum::constructor() {
        return new SmallestOfMaximum;
    }

}
