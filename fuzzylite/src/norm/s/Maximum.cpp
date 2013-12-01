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
 * File:   Maximum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:14 AM
 */

#include "fl/norm/s/Maximum.h"

namespace fl {

    std::string Maximum::className() const {
        return "Maximum";
    }

    scalar Maximum::compute(scalar a, scalar b) const {
        return Op::max(a, b);
    }

    SNorm* Maximum::constructor() {
        return new Maximum;
    }
}
