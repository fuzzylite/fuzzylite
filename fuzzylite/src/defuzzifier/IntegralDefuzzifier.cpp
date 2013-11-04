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
 * File:   IntegralDefuzzifier.cpp
 * Author: jcrada
 *
 * Created on 2 November 2013, 9:24 AM
 */

#include "fl/defuzzifier/IntegralDefuzzifier.h"

namespace fl {

    IntegralDefuzzifier::IntegralDefuzzifier(int resolution)
    : Defuzzifier(), _resolution(resolution) {
    }

    IntegralDefuzzifier::~IntegralDefuzzifier() {
    }

    void IntegralDefuzzifier::setResolution(int resolution) {
        this->_resolution = resolution;
    }

    int IntegralDefuzzifier::getResolution() const {
        return this->_resolution;
    }
}