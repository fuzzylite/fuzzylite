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
 * File:   Bisector.cpp
 * Author: jcrada
 *
 * Created on 25 April 2013, 3:56 PM
 */

#include "fl/defuzzifier/Bisector.h"

#include "fl/term/Term.h"
#include "fl/term/Accumulated.h"


namespace fl {

    Bisector::Bisector(int resolution)
    : IntegralDefuzzifier(resolution) {
    }

    std::string Bisector::className() const {
        return "Bisector";
    }

    scalar Bisector::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (maximum - minimum > _resolution) {
            FL_LOG("[accuracy warning] the resolution <" << _resolution << "> "
                    "is smaller than the range <" << minimum << ", " << maximum << ">. In order to "
                    "improve the accuracy, the resolution should be at least equal to the range.");
        }
        scalar dx = (maximum - minimum) / _resolution;

        int counter = _resolution;
        int left = 0, right = 0;
        scalar leftArea = 0, rightArea = 0;
        scalar xLeft = minimum, xRight = maximum;
        while (counter-- > 0) {
            if (fl::Op::isLE(leftArea, rightArea)) {
                xLeft = minimum + (left + 0.5) * dx;
                leftArea += term->membership(xLeft);
                left++;
            } else {
                xRight = maximum - (right + 0.5) * dx;
                rightArea += term->membership(xRight);
                right++;
            }
        }

        //Inverse weighted average to compensate
        scalar bisector = (leftArea * xRight + rightArea * xLeft) / (leftArea + rightArea);
        return bisector;
    }

    Defuzzifier* Bisector::constructor() {
        return new Bisector;
    }

}
