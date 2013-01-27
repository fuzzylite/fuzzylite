/*
 * Antecedent.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_ANTECEDENT_H
#define FL_ANTECEDENT_H

#include "fl/scalar.h"

#include <string>

namespace fl {
    class TNorm;
    class SNorm;

    class FL_EXPORT Antecedent {
    public:

        Antecedent() { }

        virtual ~Antecedent() { }

        virtual scalar firingStrength(const TNorm* tnorm, const SNorm* snorm) const = 0;

        virtual std::string toString() const = 0;
    };

}

#endif /* FL_ANTECEDENT_H */
