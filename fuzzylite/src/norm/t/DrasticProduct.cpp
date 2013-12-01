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
 * File:   DrasticProduct.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:54 AM
 */

#include "fl/norm/t/DrasticProduct.h"

namespace fl {

    std::string DrasticProduct::className() const {
        return "DrasticProduct";
    }

    scalar DrasticProduct::compute(scalar a, scalar b) const {
        if (Op::isEq(Op::max(a, b), 1.0)) {
            return Op::min(a, b);
        }
        return 0.0;
    }

    TNorm* DrasticProduct::constructor() {
        return new DrasticProduct;
    }

}