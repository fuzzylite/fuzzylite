/* 
 * File:   HamacherProduct.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:55 AM
 */

#include "fl/norm/t/HamacherProduct.h"


namespace fl {

    std::string HamacherProduct::className() const {
        return "HamacherProduct";
    }

    scalar HamacherProduct::compute(scalar a, scalar b) const {
        return (a * b) / (a + b - a * b);
    }

    TNorm* HamacherProduct::create() {
        return new HamacherProduct;
    }
}