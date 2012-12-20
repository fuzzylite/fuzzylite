/*
 * Not.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/hedge/Not.h"

namespace fl {

    std::string Not::name() const {
        return "not";
    }

    scalar Not::hedge(scalar x) const {
        return 1 - x;
    }

} /* namespace fl */
