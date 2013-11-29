/* 
 * File:   DrasticSum.cpp
 * Author: jcrada
 *
 * Created on 30 November 2013, 1:17 AM
 */

#include "fl/norm/s/DrasticSum.h"

namespace fl {

    std::string DrasticSum::className() const {
        return "DrasticSum";
    }

    scalar DrasticSum::compute(scalar a, scalar b) const {
        if (Op::isEq(Op::min(a, b), 0.0)) {
            return Op::max(a, b);
        }
        return 1.0;
    }

    SNorm* DrasticSum::create() {
        return new DrasticSum;
    }

}
