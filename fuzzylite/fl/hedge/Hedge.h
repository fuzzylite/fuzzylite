/*
 * Hedge.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_HEDGE_H
#define FL_HEDGE_H

#include "fl/scalar.h"

#include <string>

namespace fl {

    class FL_EXPORT Hedge {
    public:

        Hedge() { }

        virtual ~Hedge() { }

        virtual std::string name() const = 0;
        virtual scalar hedge(scalar x) const = 0;

    };
}

#endif /* FL_HEDGE_H */
