/* 
 * File:   AlgebraicProduct.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:53 AM
 */

#include "fl/norm/t/AlgebraicProduct.h"

namespace fl {

    std::string AlgebraicProduct::className() const {
        return "AlgebraicProduct";
    }

    scalar AlgebraicProduct::compute(scalar a, scalar b) const {
        return a * b;
    }

    TNorm* AlgebraicProduct::create() {
        return new AlgebraicProduct;
    }

}
