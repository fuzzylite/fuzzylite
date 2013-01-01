/* 
 * File:   TNorm.cpp
 * Author: jcrada
 * 
 * Created on 21 December 2012, 9:33 AM
 */

#include "fl/operator/TNorm.h"

namespace fl {

    // Minimum : public TNorm {

    std::string Minimum::className() const {
        return "Minimum";
    }

    scalar Minimum::compute(scalar a, scalar b) const {
        return Op::Min(a, b);
    }

    // Product : public TNorm {

    std::string AlgebraicProduct::className() const {
        return "AlgebraicProduct";
    }

    scalar AlgebraicProduct::compute(scalar a, scalar b) const {
        return a * b;
    }

    // BoundedDifference : public TNorm {

    std::string BoundedDifference::className() const {
        return "BoundedDifference";
    }

    scalar BoundedDifference::compute(scalar a, scalar b) const {
        return Op::Max(0, a + b - 1);
    }


    // DrasticProduct : public TNorm {

    std::string DrasticProduct::className() const {
        return "DrasticProduct";
    }

    scalar DrasticProduct::compute(scalar a, scalar b) const {
        if (Op::IsEq(Op::Max(a, b), 1.0)) {
            return Op::Min(a, b);
        }
        return 0.0;
    }


    // EinsteinProduct : public TNorm {

    std::string EinsteinProduct::className() const {
        return "EinsteinProduct";
    }

    scalar EinsteinProduct::compute(scalar a, scalar b) const {
        return (a * b) / (2 - (a + b - a * b));
    }


    // HamacherProduct : public TNorm {

    std::string HamacherProduct::className() const {
        return "HamacherProduct";
    }

    scalar HamacherProduct::compute(scalar a, scalar b) const {
        return (a * b) / (a + b - a * b);
    }


}
