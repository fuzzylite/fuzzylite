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
 * File:   PiShape.cpp
 * Author: jcrada
 * 
 * Created on 22 December 2012, 5:15 PM
 */

#include "fl/term/PiShape.h"

namespace fl {

    PiShape::PiShape(const std::string& name, scalar bottomLeft, scalar topLeft,
            scalar topRight, scalar bottomRight)
    : Term(name), _bottomLeft(bottomLeft), _topLeft(topLeft),
    _topRight(topRight), _bottomRight(bottomRight) {
    }

    PiShape::~PiShape() {
    }

    std::string PiShape::className() const {
        return "PiShape";
    }

    std::string PiShape::parameters() const {
        return Op::join(4, " ", _bottomLeft, _topLeft, _topRight, _bottomRight);
    }

    void PiShape::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 4;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setBottomLeft(Op::toScalar(values.at(0)));
        setTopLeft(Op::toScalar(values.at(1)));
        setTopRight(Op::toScalar(values.at(2)));
        setBottomRight(Op::toScalar(values.at(3)));
    }

    scalar PiShape::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        //from Octave smf.m
        scalar a_b_ave = (_bottomLeft + _topLeft) / 2.0;
        scalar b_minus_a = _topLeft - _bottomLeft;
        scalar c_d_ave = (_topRight + _bottomRight) / 2.0;
        scalar d_minus_c = _bottomRight - _topRight;

        if (Op::isLE(x, _bottomLeft)) return 0.0;

        else if (Op::isLE(x, a_b_ave))
            return 2.0 * std::pow((x - _bottomLeft) / b_minus_a, 2);

        else if (Op::isLt(x, _topLeft))
            return 1.0 - 2.0 * std::pow((x - _topLeft) / b_minus_a, 2);

        else if (Op::isLE(x, _topRight))
            return 1;

        else if (Op::isLE(x, c_d_ave))
            return 1 - 2 * std::pow((x - _topRight) / d_minus_c, 2);

        else if (Op::isLt(x, _bottomRight))
            return 2 * std::pow((x - _bottomRight) / d_minus_c, 2);

        return 0.0;

    }

    void PiShape::setBottomLeft(scalar a) {
        this->_bottomLeft = a;
    }

    scalar PiShape::getBottomLeft() const {
        return this->_bottomLeft;
    }

    void PiShape::setTopLeft(scalar b) {
        this->_topLeft = b;
    }

    scalar PiShape::getTopLeft() const {
        return this->_topLeft;
    }

    void PiShape::setTopRight(scalar d) {
        this->_topRight = d;
    }

    scalar PiShape::getTopRight() const {
        return this->_topRight;
    }

    void PiShape::setBottomRight(scalar c) {
        this->_bottomRight = c;
    }

    scalar PiShape::getBottomRight() const {
        return this->_bottomRight;
    }

    PiShape* PiShape::copy() const {
        return new PiShape(*this);
    }

    Term* PiShape::constructor() {
        return new PiShape;
    }
}