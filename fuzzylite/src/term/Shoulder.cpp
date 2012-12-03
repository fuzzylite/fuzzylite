/*
 * Shoulder.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Shoulder.h"

#include "../engine/Operator.h"

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

    scalar LeftShoulder::membership(scalar x) const{
        if (Op::IsLt(x, _minimum)) return 0.0;
        if (Op::IsGt(x, _maximum)) return 1.0;
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

    /**
     * Right Shoulder
     */
    RightShoulder::RightShoulder(const std::string& name, scalar minimum,
            scalar maximum)
            : Term(name), _minimum(minimum), _maximum(maximum) {
    }

    RightShoulder::~RightShoulder() {

    }

    scalar RightShoulder::membership(scalar x) const{
        if (Op::IsLt(x, _minimum)) return 1.0;
        if (Op::IsGt(x, _maximum)) return 0.0;
        return (x - _minimum) / (_maximum - _minimum);

    }

    std::string RightShoulder::toString() const {
        std::stringstream ss;
        ss << "RightShoulder (" << _minimum << ", " << _maximum << ")";
        return ss.str();
    }

    void RightShoulder::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }
    scalar RightShoulder::minimum() const {
        return this->_minimum;
    }
    void RightShoulder::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }
    scalar RightShoulder::maximum() const {
        return this->_maximum;
    }

} /* namespace fl */
