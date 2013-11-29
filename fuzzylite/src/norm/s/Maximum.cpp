/* 
 * File:   Maximum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:14 AM
 */

#include "fl/norm/s/Maximum.h"

namespace fl {

    std::string Maximum::className() const {
        return "Maximum";
    }

    scalar Maximum::compute(scalar a, scalar b) const {
        return Op::max(a, b);
    }

    SNorm* Maximum::create() {
        return new Maximum;
    }
}
