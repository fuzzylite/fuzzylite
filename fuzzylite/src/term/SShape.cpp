/* 
 * File:   SShape.cpp
 * Author: jcrada
 * 
 * Created on 22 December 2012, 4:43 PM
 */

#include "fl/term/SShape.h"
#include "fl/operator/Operator.h"

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
        //from Octave smf.m
        scalar average = (_start + _end) / 2;
        scalar difference = _end - _start;

        if (Op::IsLE(x, _start)) return 0.0;

        else if (Op::IsLE(x, average))
            return 2 * std::pow((x - _start) / difference, 2);

        else if (Op::IsLt(x, _end))
            return 1.0 - 2.0 * std::pow((x - _end) / difference, 2);

        return 1.0;
    }

    std::string SShape::toString() const {
        std::ostringstream ss;
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