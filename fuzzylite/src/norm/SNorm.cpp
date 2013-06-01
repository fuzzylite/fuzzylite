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
 * File:   SNorm.cpp
 * Author: jcrada
 * 
 * Created on 21 December 2012, 9:34 AM
 */

#include "fl/norm/SNorm.h"

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
        return Op::min(1.0, a + b);
    }

    std::string NormalizedSum::className() const {
        return "NormalizedSum";
    }

    scalar NormalizedSum::compute(scalar a, scalar b) const {
        return a + b / Op::max(1.0, Op::max(a, b));
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
        return (a + b) / (1.0 + a * b);
    }

    std::string HamacherSum::className() const {
        return "HamacherSum";
    }

    scalar HamacherSum::compute(scalar a, scalar b) const {
        return (a + b - 2 * a * b) / (1 - a * b);
    }


}
