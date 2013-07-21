/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * MeanOfMaximum.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/defuzzifier/MeanOfMaximum.h"

#include "fl/term/Term.h"

#include "fl/Exception.h"




namespace fl {

    MeanOfMaximum::MeanOfMaximum(int divisions)
    : Defuzzifier(divisions) { }

    MeanOfMaximum::~MeanOfMaximum() { }

    std::string MeanOfMaximum::className() const {
        return "MeanOfMaximum";
    }

    scalar MeanOfMaximum::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (maximum - minimum > _divisions) {
            FL_LOG("[accuracy warning] the number of divisions ( " << _divisions << ") "
                    "is less than the range (" << minimum << ", " << maximum << "). In order to "
                    "improve the accuracy, the number of divisions should be greater than the range.");
        }
        scalar dx = (maximum - minimum) / _divisions;
        scalar x, y;
        scalar ymax = -1.0;
        scalar xsmallest = minimum;
        scalar xlargest = maximum;
        bool samePlateau = false;
        for (int i = 0; i < _divisions; ++i) {
            x = minimum + (i + 0.5) * dx;
            y = term->membership(x);

            if (Op::isGt(y, ymax)) {
                ymax = y;

                xsmallest = x;
                xlargest = x;

                samePlateau = true;
            } else if (Op::isEq(y, ymax) and samePlateau) {
                xlargest = x;
            } else if (Op::isLt(y, ymax)) {
                samePlateau = false;
            }
        }

        return (xlargest + xsmallest) / 2.0;
    }
}
