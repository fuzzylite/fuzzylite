/*
 * MaximumDefuzzifier.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/defuzzifier/MaximumDefuzzifier.h"

#include "fl/term/Term.h"

#include "fl/operator/Operator.h"
#include "fl/Exception.h"

#include "fl/definitions.h"


namespace fl {

    MaximumDefuzzifier::MaximumDefuzzifier(Type type, int divisions)
    : Defuzzifier(divisions), _type(type) { }

    std::string MaximumDefuzzifier::className() const {
        switch (_type) {
            case SMALLEST:
                return "SmallestOfMaximum";
            case LARGEST:
                return "LargestOfMaximum";
            case MEAN:
                return "MeanOfMaximum";
            default:
                throw fl::Exception("[internal error]");
        }
    }

    scalar MaximumDefuzzifier::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (maximum - minimum > _divisions) {
            FL_LOG("[accuracy warning] the number of divisions ( " << _divisions << ") "
                    "is less than the range (" << minimum << ", " << maximum << "). In order to "
                    "improve the accuracy, the number of divisions should be greater than the range.");
        }
        scalar dx = (maximum - minimum) / _divisions;
        scalar x, y;
        scalar ymax = -1.0, xsmallest, xlargest;
        bool samePlateau = false;
        for (int i = 0; i < _divisions; ++i) {
            x = minimum + (i + 0.5) * dx;
            y = term->membership(x);

            if (Op::isGt(y, ymax)) {
                xsmallest = x;
                ymax = y;
                samePlateau = true;
            } else if (Op::isEq(y, ymax) and samePlateau) {
                xlargest = x;
            } else if (Op::isLt(y, ymax)) {
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

}
