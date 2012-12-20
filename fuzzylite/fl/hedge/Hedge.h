/*
 * Hedge.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_HEDGE_H_
#define FL_HEDGE_H_

#include "fl/scalar.h"

#include <string>

namespace fl {

    class Hedge {
    public:

        Hedge() { }

        virtual ~Hedge() { }

        virtual std::string name() const = 0;
        virtual scalar hedge(scalar x) const = 0;

    };
}

#endif /* FL_HEDGE_H_ */
