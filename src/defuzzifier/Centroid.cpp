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

#include "fuzzylite/defuzzifier/Centroid.h"

#include "fuzzylite/term/Term.h"

namespace fuzzylite {

    Centroid::Centroid(int resolution) : IntegralDefuzzifier(resolution) {}

    Centroid::~Centroid() {}

    std::string Centroid::className() const {
        return "Centroid";
    }

    scalar Centroid::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (not Op::isFinite(minimum + maximum))
            return fl::nan;

        const int resolution = getResolution();
        const scalar dx = (maximum - minimum) / resolution;
        scalar area = 0.0, centroid = 0.0;
        for (int i = 0; i < resolution; ++i) {
            const scalar x = minimum + (i + 0.5) * dx;
            const scalar y = term->membership(x);

            centroid += y * x;
            area += y;
        }
        centroid /= area;
        return centroid;
    }

    Centroid* Centroid::clone() const {
        return new Centroid(*this);
    }

    Defuzzifier* Centroid::constructor() {
        return new Centroid;
    }

}
