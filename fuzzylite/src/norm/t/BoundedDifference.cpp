/* 
 * File:   BoundedDifference.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:54 AM
 */

#include "fl/norm/t/BoundedDifference.h"


namespace fl {

    std::string BoundedDifference::className() const {
        return "BoundedDifference";
    }

    scalar BoundedDifference::compute(scalar a, scalar b) const {
        return Op::max(0.0, a + b - 1);
    }

    TNorm* BoundedDifference::create() {
        return new BoundedDifference;
    }
}