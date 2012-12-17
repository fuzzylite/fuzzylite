/*
 * LeftShoulder.cpp
 *
 *  Created on: 11/12/2012
 *      Author: jcrada
 */

#include "fl/term/LeftShoulder.h"

#include "fl/engine/Operator.h"

#include <sstream>

namespace fl {

    /**
     * Left Shoulder
     */
    LeftShoulder::LeftShoulder(const std::string& name, scalar minimum,
            scalar maximum)
            : Term(name), _minimum(minimum), _maximum(maximum) {
    }

    LeftShoulder::~LeftShoulder() {
    }

    std::string LeftShoulder::className() const{
        return "LeftShoulder";
    }

    LeftShoulder* LeftShoulder::copy() const{
        return new LeftShoulder(*this);
    }

    scalar LeftShoulder::membership(scalar x) const {
        if (Op::IsLt(x, _minimum)) return 1.0;
        if (Op::IsGt(x, _maximum)) return 0.0;
        return (_maximum - x) / (_maximum - _minimum);
    }

    std::string LeftShoulder::toString() const {
        std::stringstream ss;
        ss << "LeftShoulder (" << _minimum << ", " << _maximum << ")";
        return ss.str();
    }

    void LeftShoulder::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }
    scalar LeftShoulder::minimum() const {
        return this->_minimum;
    }
    void LeftShoulder::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }
    scalar LeftShoulder::maximum() const {
        return this->_maximum;
    }

}

