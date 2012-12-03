/*
 * MamdaniConsequent.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANICONSEQUENT_H_
#define FL_MAMDANICONSEQUENT_H_

#include "../Consequent.h"

namespace fl {
    class Engine;

    class MamdaniConsequent: public Consequent {
    public:
        MamdaniConsequent();
        ~MamdaniConsequent();

        void load(const std::string& consequent, const Engine* engine);

        void fire(scalar strength, const Operator* activation);

        std::string toString() const;
    };

} /* namespace fl */
#endif /* FL_MAMDANICONSEQUENT_H_ */
