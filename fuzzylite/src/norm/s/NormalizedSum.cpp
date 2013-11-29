/* 
 * File:   NormalizedSum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:16 AM
 */

#include "fl/norm/s/NormalizedSum.h"

namespace fl {

    std::string NormalizedSum::className() const {
        return "NormalizedSum";
    }

    scalar NormalizedSum::compute(scalar a, scalar b) const {
        return a + b / Op::max(1.0, Op::max(a, b));
    }

    SNorm* NormalizedSum::create() {
        return new NormalizedSum;
    }

}
