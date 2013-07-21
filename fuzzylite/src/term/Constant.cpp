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

#include "fl/term/Constant.h"

namespace fl {

    Constant::Constant(const std::string& name, scalar value)
    : Term(name), _value(value) { }

    Constant::~Constant() { }

    std::string Constant::className() const {
        return "Constant";
    }

    scalar Constant::membership(scalar x) const {
        (void) x;
        return this->_value;
    }

    Constant* Constant::copy() const {
        return new Constant(*this);
    }

    std::string Constant::toString() const {
        std::ostringstream ss;
        ss << className() << "(" << fl::Op::str(this->_value) << ")";
        return ss.str();
    }

    void Constant::setValue(scalar value) {
        this->_value = value;
    }

    scalar Constant::getValue() const {
        return this->_value;
    }

}
