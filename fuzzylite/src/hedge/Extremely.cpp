/*
 * Extremely.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/hedge/Extremely.h"

#include "fl/operator/Operator.h"

namespace fl {

    std::string Extremely::name() const {
        return "extremely";
    }

    scalar Extremely::hedge(scalar x) const {
        return Op::IsLE(x, 0.5)
                ? 2 * x * x
                : 1 - 2 * (1 - x) * (1 - x);
    }
} 
