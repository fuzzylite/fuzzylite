/*
 * Antecedent.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_ANTECEDENT_H_
#define FL_ANTECEDENT_H_

#include "fl/scalar.h"

#include <string>

namespace fl {
    class TNorm;
    class SNorm;

    class Antecedent {
    public:

        Antecedent() { }

        virtual ~Antecedent() { }

        virtual scalar firingStrength(const TNorm* tnorm, const SNorm* snorm) const = 0;

        virtual std::string toString() const = 0;
    };

}

#endif /* FL_ANTECEDENT_H_ */
