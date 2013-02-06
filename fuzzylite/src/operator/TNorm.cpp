/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
        return Op::min(a, b);
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
        return Op::max(0, a + b - 1);
    }


    // DrasticProduct : public TNorm {

    std::string DrasticProduct::className() const {
        return "DrasticProduct";
    }

    scalar DrasticProduct::compute(scalar a, scalar b) const {
        if (Op::isEq(Op::max(a, b), 1.0)) {
            return Op::min(a, b);
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
