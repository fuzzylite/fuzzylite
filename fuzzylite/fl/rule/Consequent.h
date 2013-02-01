/*
 * Consequent.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CONSEQUENT_H
#define FL_CONSEQUENT_H

#include "fl/fuzzylite.h"



#include <string>

namespace fl {
    class TNorm;

    class FL_EXPORT Consequent {
    public:

        Consequent() { }

        virtual ~Consequent() { }

        virtual void fire(scalar strength, const TNorm* activation) = 0;

        virtual std::string toString() const = 0;

    };

}

#endif /* FL_CONSEQUENT_H */
