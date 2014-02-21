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
 * Defuzzifier.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */


//TODO Check http://en.wikipedia.org/wiki/Defuzzification for other defuzzifiers.

#ifndef FL_DEFUZZIFIER_H
#define FL_DEFUZZIFIER_H

#include "fl/fuzzylite.h"
#include <string>

namespace fl {
    class Term;

    class FL_EXPORT Defuzzifier {
    public:

        Defuzzifier() {
        }

        virtual ~Defuzzifier() {
        }

        virtual std::string className() const = 0;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const = 0;

    };

}
#endif /* FL_DEFUZZIFIER_H */
