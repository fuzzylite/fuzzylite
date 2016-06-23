/*
 Copyright © 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/defuzzifier/Centroid.h"

#include "fl/term/Aggregated.h"
#include "fl/term/Term.h"

namespace fl {

    Centroid::Centroid(int resolution)
    : IntegralDefuzzifier(resolution) {
    }

    Centroid::~Centroid() {

    }

    std::string Centroid::className() const {
        return "Centroid";
    }

    scalar Centroid::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (not Op::isFinite(minimum + maximum)) return fl::nan;

        const int resolution = getResolution();
        scalar dx = (maximum - minimum) / resolution;
        scalar x, y;
        scalar area = 0, xcentroid = 0, ycentroid = 0;
        for (int i = 0; i < resolution; ++i) {
            x = minimum + (i + 0.5) * dx;
            y = term->membership(x);

            xcentroid += y * x;
            ycentroid += y * y;
            area += y;
        }
        //Final results not computed for efficiency
        //xcentroid /= area;
        //ycentroid /= 2 * area;
        //area *= dx;
        return xcentroid / area;
    }

    Centroid* Centroid::clone() const {
        return new Centroid(*this);
    }

    Defuzzifier* Centroid::constructor() {
        return new Centroid;
    }

}
