/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/defuzzifier/SmallestOfMaximum.h"

#include "fl/Exception.h"
#include "fl/term/Term.h"


namespace fl {

    SmallestOfMaximum::SmallestOfMaximum(int resolution)
    : IntegralDefuzzifier(resolution) {
    }

    SmallestOfMaximum::~SmallestOfMaximum() {
    }

    std::string SmallestOfMaximum::className() const {
        return "SmallestOfMaximum";
    }

    scalar SmallestOfMaximum::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (not fl::Op::isFinite(minimum + maximum)) {
            return fl::nan;
        }
        if (maximum - minimum > getResolution()) {
            FL_DBG("[accuracy warning] the resolution <" << getResolution() << "> "
                    "is smaller than the range <" << minimum << ", " << maximum << ">. In order to "
                    "improve the accuracy, the resolution should be at least equal to the range.");
        }
        scalar dx = (maximum - minimum) / getResolution();
        scalar x, y;
        scalar ymax = -1.0, xsmallest = minimum;
        for (int i = 0; i < getResolution(); ++i) {
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

