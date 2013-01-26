/*
 * MamdaniConsequent.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANICONSEQUENT_H
#define FL_MAMDANICONSEQUENT_H

#include "fl/rule/Consequent.h"

#include <vector>

namespace fl {
    class Engine;
    class MamdaniProposition;

    flclass MamdaniConsequent : public Consequent {
    protected:
        std::vector<MamdaniProposition*> _conclusions;

    public:
        MamdaniConsequent();
        virtual ~MamdaniConsequent();
        
        virtual std::vector<MamdaniProposition*> conclusions() const;
        
        virtual void load(const std::string& consequent, const Engine* engine);

        virtual void fire(scalar strength, const TNorm* activation);

        virtual std::string toString() const;

    };

} 
#endif /* FL_MAMDANICONSEQUENT_H */
