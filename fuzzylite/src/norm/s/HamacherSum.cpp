/* 
 * File:   HamacherSum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:17 AM
 */

#include "fl/norm/s/HamacherSum.h"

namespace fl {

    std::string HamacherSum::className() const {
        return "HamacherSum";
    }

    scalar HamacherSum::compute(scalar a, scalar b) const {
        return (a + b - 2 * a * b) / (1 - a * b);
    }

    SNorm* HamacherSum::create() {
        return new HamacherSum;
    }
}
