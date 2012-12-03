/*
 * MamdaniAntecedent.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIANTECEDENT_H_
#define FL_MAMDANIANTECEDENT_H_

#include "../Antecedent.h"

namespace fl {
    class Engine;

    class MamdaniAntecedent: public Antecedent {
    public:
        MamdaniAntecedent();
        ~MamdaniAntecedent();

        void load(const std::string& antecedent, const Engine* engine);

        scalar firingStrength(const Operator* tnorm, const Operator* snorm) const;

        std::string toString() const;
    };

} /* namespace fl */
#endif /* FL_MAMDANIANTECEDENT_H_ */
