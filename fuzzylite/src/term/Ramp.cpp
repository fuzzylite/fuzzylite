/* 
 * File:   Ramp.cpp
 * Author: jcrada
 * 
 * Created on 22 December 2012, 4:27 PM
 */

#include "fl/term/Ramp.h"
#include "fl/operator/Operator.h"

namespace fl {

    Ramp::Ramp(const std::string& name, scalar start, scalar end)
    : Term(name), _start(start), _end(end) {
    }

    Ramp::~Ramp() {
    }

    std::string Ramp::className() const {
        return "Ramp";
    }

    Ramp* Ramp::copy() const {
        return new Ramp(*this);
    }

    scalar Ramp::membership(scalar x) const {
        if (Op::isEq(_start, _end)) return 0.0;

        if (Op::isLt(_start, _end)) {
            if (Op::isLt(x, _start)) return 0.0;
            if (Op::isGt(x, _end)) return 1.0;
            return (x - _start) / (_end - _start);
        } else {
            if (Op::isLt(x, _start)) return 1.0;
            if (Op::isGt(x, _end)) return 0.0;
            return (_end - x) / (_end - _start);
        }
    }

    std::string Ramp::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _start << ", " << _end << ")";
        return ss.str();
    }

    void Ramp::setStart(scalar start) {
        this->_start = start;
    }

    scalar Ramp::getStart() const {
        return this->_start;
    }

    void Ramp::setEnd(scalar end) {
        this->_end = end;
    }

    scalar Ramp::getEnd() const {
        return this->_end;
    }

}