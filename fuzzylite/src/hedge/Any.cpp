/*
 * Any.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/hedge/Any.h"

namespace fl {

    std::string Any::name() const {
        return "any";
    }

    scalar Any::hedge(scalar x) const {
        return 1.0;
    }

} 
