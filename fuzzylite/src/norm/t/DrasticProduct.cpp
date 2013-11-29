/* 
 * File:   DrasticProduct.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:54 AM
 */

#include "fl/norm/t/DrasticProduct.h"

namespace fl {

    std::string DrasticProduct::className() const {
        return "DrasticProduct";
    }

    scalar DrasticProduct::compute(scalar a, scalar b) const {
        if (Op::isEq(Op::max(a, b), 1.0)) {
            return Op::min(a, b);
        }
        return 0.0;
    }

    TNorm* DrasticProduct::create() {
        return new DrasticProduct;
    }

}