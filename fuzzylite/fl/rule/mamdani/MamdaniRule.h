/*
 * MamdaniRule.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIRULE_H
#define FL_MAMDANIRULE_H

#include "fl/fuzzylite.h"

#include "fl/rule/Rule.h"

//Needed here for covariant return
#include "fl/rule/mamdani/MamdaniAntecedent.h"
#include "fl/rule/mamdani/MamdaniConsequent.h"

#include <string>

namespace fl {
    class MamdaniAntecedent;
    class MamdaniConsequent;
    class Engine;
    
    class FL_EXPORT MamdaniRule : public Rule {
    protected:
        MamdaniAntecedent* _antecedent;
        MamdaniConsequent* _consequent;
    public:
        MamdaniRule();
        ~MamdaniRule();

        void setAntecedent(MamdaniAntecedent* antecedent);
        MamdaniAntecedent* getAntecedent() const;

        void setConsequent(MamdaniConsequent* consequent);
        MamdaniConsequent* getConsequent() const;

        static MamdaniRule* parse(const std::string& rule, const Engine* engine);
    };

}
#endif /* FL_MAMDANIRULE_H */
