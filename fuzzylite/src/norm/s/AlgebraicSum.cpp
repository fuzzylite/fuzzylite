/* 
 * File:   AlgebraicSum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:15 AM
 */

#include "fl/norm/s/AlgebraicSum.h"

namespace fl {

    std::string AlgebraicSum::className() const {
        return "AlgebraicSum";
    }

    scalar AlgebraicSum::compute(scalar a, scalar b) const {
        return a + b - (a * b);
    }

    SNorm* AlgebraicSum::create() {
        return new AlgebraicSum;
    }
}