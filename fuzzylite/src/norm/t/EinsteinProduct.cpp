/* 
 * File:   EinsteinProduct.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:55 AM
 */

#include "fl/norm/t/EinsteinProduct.h"

namespace fl {

    std::string EinsteinProduct::className() const {
        return "EinsteinProduct";
    }

    scalar EinsteinProduct::compute(scalar a, scalar b) const {
        return (a * b) / (2 - (a + b - a * b));
    }

    TNorm* EinsteinProduct::create() {
        return new EinsteinProduct;
    }

}