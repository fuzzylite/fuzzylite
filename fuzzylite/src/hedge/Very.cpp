/*
 * Very.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/hedge/Very.h"

namespace fl {

    std::string Very::name() const {
        return "very";
    }

    scalar Very::hedge(scalar x) const {
        return x * x;
    }

} /* namespace fl */
