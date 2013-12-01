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
 * Any.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/hedge/Any.h"

namespace fl {

    std::string Any::name() const {
        return "any";
    }

    scalar Any::hedge(scalar x) const {
        (void) x;
        return 1.0;
    }
    
    Hedge* Any::constructor(){
        return new Any;
    }

}
