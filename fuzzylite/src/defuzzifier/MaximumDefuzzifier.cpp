/*
 * MaximumDefuzzifier.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/defuzzifier/MaximumDefuzzifier.h"

#include "fl/term/Term.h"

#include "fl/engine/Operator.h"


namespace fl {

MaximumDefuzzifier::MaximumDefuzzifier(Type type, int divisions)
        : Defuzzifier(divisions), _type(type) {
}

std::string MaximumDefuzzifier::name() const {
    switch (_type) {
        case SMALLEST:
            return "SOM";
        case LARGEST:
            return "LOM";
        case MEAN:
            return "MOM";
        default:
            return "???";
    }
}

scalar MaximumDefuzzifier::defuzzify(const Term* term) const {
    scalar dx = (term->maximum() - term->minimum()) / _divisions;
    scalar x, y;
    scalar ymax = -1.0, xsmallest, xlargest;
    bool samePlateau = false;
    for (int i = 0; i < _divisions; ++i) {
        x = term->minimum() + (i + 0.5) * dx;
        y = term->membership(x);

        if (Op::IsGt(y, ymax)) {
            xsmallest = x;
            ymax = y;
            samePlateau = true;
        } else if (Op::IsEq(y, ymax)  and  samePlateau) {
            xlargest = x;
        } else if (Op::IsLt(y, ymax)) {
            samePlateau = false;
        }
    }

    switch (_type) {
        case SMALLEST:
            return xsmallest;
        case LARGEST:
            return xlargest;
        case MEAN:
            return (xlargest + xsmallest) / 2.0;
        default:
            return std::numeric_limits<scalar>::quiet_NaN();
    }
}

} /* namespace fl */
