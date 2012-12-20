/*
 * Somewhat.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/hedge/Somewhat.h"

#include <cmath>

namespace fl {

    std::string Somewhat::name() const {
        return "somewhat";
    }

    scalar Somewhat::hedge(scalar x) const {
        return std::sqrt(x);
    }

} /* namespace fl */
