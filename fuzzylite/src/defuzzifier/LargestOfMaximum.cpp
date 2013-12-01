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
 * LargestOfMaximum.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/defuzzifier/LargestOfMaximum.h"

#include "fl/term/Term.h"

#include "fl/Exception.h"




namespace fl {

    LargestOfMaximum::LargestOfMaximum(int resolution)
    : IntegralDefuzzifier(resolution) {
    }

    LargestOfMaximum::~LargestOfMaximum() {
    }

    std::string LargestOfMaximum::className() const {
        return "LargestOfMaximum";
    }

    scalar LargestOfMaximum::defuzzify(const Term* term, scalar minimum, scalar maximum) const {
        if (maximum - minimum > _resolution) {
            FL_LOG("[accuracy warning] the resolution <" << _resolution << "> "
                    "is smaller than the range <" << minimum << ", " << maximum << ">. In order to "
                    "improve the accuracy, the resolution should be at least equal to the range.");
        }
        scalar dx = (maximum - minimum) / _resolution;
        scalar x, y;
        scalar ymax = -1.0, xlargest = maximum;
        for (int i = 0; i < _resolution; ++i) {
            x = minimum + (i + 0.5) * dx;
            y = term->membership(x);

            if (Op::isGE(y, ymax)) {
                ymax = y;
                xlargest = x;
            }
        }
        return xlargest;
    }

    Defuzzifier* LargestOfMaximum::constructor() {
        return new LargestOfMaximum;
    }
}
