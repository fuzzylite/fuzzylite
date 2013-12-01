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
 * Seldom.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/hedge/Seldom.h"
#include "fl/Operation.h"

#include <cmath>

namespace fl {

    std::string Seldom::name() const {
        return "seldom";
    }

    scalar Seldom::hedge(scalar x) const {
        return Op::isLE(x, 0.5)
                ? std::sqrt(x / 2)
                : 1 - std::sqrt((1 - x) / 2);
    }
    
    Hedge* Seldom::constructor(){
        return new Seldom;
    }

} 
