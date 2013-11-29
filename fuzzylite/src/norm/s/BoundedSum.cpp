/* 
 * File:   BoundedSum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:16 AM
 */

#include "fl/norm/s/BoundedSum.h"

namespace fl {

    std::string BoundedSum::className() const {
        return "BoundedSum";
    }

    scalar BoundedSum::compute(scalar a, scalar b) const {
        return Op::min(1.0, a + b);
    }

    SNorm* BoundedSum::create() {
        return new BoundedSum;
    }
}
