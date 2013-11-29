/* 
 * File:   Minimum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:52 AM
 */

#include "fl/norm/t/Minimum.h"

namespace fl {

    std::string Minimum::className() const {
        return "Minimum";
    }

    scalar Minimum::compute(scalar a, scalar b) const {
        return Op::min(a, b);
    }

    TNorm* Minimum::create() {
        return new Minimum;
    }


}
