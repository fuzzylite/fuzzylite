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

#include "fl/defuzzifier/LargestOfMaximum.h"

#include "fl/Exception.h"
#include "fl/term/Term.h"

namespace fl {

    LargestOfMaximum::LargestOfMaximum(int resolution)
    : IntegralDefuzzifier(resolution) { }

    LargestOfMaximum::~LargestOfMaximum() { }

    std::string LargestOfMaximum::className() const {
        return "LargestOfMaximum";
    }

    Complexity LargestOfMaximum::complexity(const Term* term) const {
        return Complexity().comparison(1).arithmetic(1 + 2) +
                term->complexity().comparison(1).arithmetic(3).multiply(getResolution());
    }

    scalar LargestOfMaximum::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (not Op::isFinite(minimum + maximum)) return fl::nan;

        const int resolution = getResolution();
        const scalar dx = (maximum - minimum) / resolution;
        scalar x, y;
        scalar ymax = -1.0, xlargest = maximum;
        for (int i = 0; i < resolution; ++i) {
            x = minimum + (i + 0.5) * dx;
            y = term->membership(x);

            if (Op::isGE(y, ymax)) {
                ymax = y;
                xlargest = x;
            }
        }
        return xlargest;
    }

    LargestOfMaximum* LargestOfMaximum::clone() const {
        return new LargestOfMaximum(*this);
    }

    Defuzzifier* LargestOfMaximum::constructor() {
        return new LargestOfMaximum;
    }

}
