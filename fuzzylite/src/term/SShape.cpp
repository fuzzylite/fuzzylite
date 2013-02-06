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
 * File:   SShape.cpp
 * Author: jcrada
 * 
 * Created on 22 December 2012, 4:43 PM
 */

#include "fl/term/SShape.h"

namespace fl {

    SShape::SShape(const std::string& name, scalar start, scalar end)
    : Term(name), _start(start), _end(end) {
    }

    std::string SShape::className() const {
        return "SShape";
    }

    SShape* SShape::copy() const {
        return new SShape(*this);
    }

    scalar SShape::membership(scalar x) const {
        if (fl::Op::isNan(x)) return std::numeric_limits<scalar>::quiet_NaN();
        //from Octave smf.m
        scalar average = (_start + _end) / 2;
        scalar difference = _end - _start;

        if (Op::isLE(x, _start)) return 0.0;

        else if (Op::isLE(x, average))
            return 2 * std::pow((x - _start) / difference, 2);

        else if (Op::isLt(x, _end))
            return 1.0 - 2.0 * std::pow((x - _end) / difference, 2);

        return 1.0;
    }

    std::string SShape::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _start << ", " << _end << ")";
        return ss.str();
    }

    void SShape::setStart(scalar start) {
        this->_start = start;
    }

    scalar SShape::getStart() const {
        return this->_start;
    }

    void SShape::setEnd(scalar end) {
        this->_end = end;
    }

    scalar SShape::getEnd() const {
        return this->_end;
    }




}