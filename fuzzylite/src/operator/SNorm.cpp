/* 
 * File:   SNorm.cpp
 * Author: jcrada
 * 
 * Created on 21 December 2012, 9:34 AM
 */

#include "fl/operator/SNorm.h"

namespace fl {

    std::string Maximum::className() const {
        return "Maximum";
    }

    scalar Maximum::compute(scalar a, scalar b) const {
        return Op::max(a, b);
    }

    std::string AlgebraicSum::className() const {
        return "AlgebraicSum";
    }

    scalar AlgebraicSum::compute(scalar a, scalar b) const {
        return a + b - (a * b);
    }

    std::string BoundedSum::className() const {
        return "BoundedSum";
    }

    scalar BoundedSum::compute(scalar a, scalar b) const {
        return Op::min(1, a + b);
    }

    std::string NormalizedSum::className() const {
        return "NormalizedSum";
    }

    scalar NormalizedSum::compute(scalar a, scalar b) const {
        return a + b / Op::max(1, Op::max(a, b));
    }

    std::string DrasticSum::className() const {
        return "DrasticSum";
    }

    scalar DrasticSum::compute(scalar a, scalar b) const {
        if (Op::isEq(Op::min(a, b), 0.0)) {
            return Op::max(a, b);
        }
        return 1.0;
    }

    std::string EinsteinSum::className() const {
        return "EinsteinSum";
    }

    scalar EinsteinSum::compute(scalar a, scalar b) const {
        return (a + b) / (1 + a * b);
    }

    std::string HamacherSum::className() const {
        return "HamacherSum";
    }

    scalar HamacherSum::compute(scalar a, scalar b) const {
        return (a + b - 2 * a * b) / (1 - a * b);
    }


}
