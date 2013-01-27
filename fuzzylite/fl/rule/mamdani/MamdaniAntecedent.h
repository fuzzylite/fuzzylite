/*
 * MamdaniAntecedent.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIANTECEDENT_H
#define FL_MAMDANIANTECEDENT_H

#include "fl/rule/Antecedent.h"

#include <sstream>
#include <vector>

namespace fl {
    class Engine;
    class TNorm;
    class SNorm;
    class MamdaniExpression;

    class FL_EXPORT MamdaniAntecedent : public Antecedent {
    protected:
        MamdaniExpression* _root;

    public:
        MamdaniAntecedent();
        virtual ~MamdaniAntecedent();

        virtual MamdaniExpression* getRoot() const;
        
        virtual void load(const std::string& antecedent, const Engine* engine);

        virtual scalar firingStrength(const TNorm* tnorm, const SNorm* snorm, const MamdaniExpression* node) const;

        virtual scalar firingStrength(const TNorm* tnorm, const SNorm* snorm) const;

        virtual std::string toString() const;

        virtual std::string toStringPrefix(const MamdaniExpression* node = NULL) const;
        virtual std::string toStringInfix(const MamdaniExpression* node = NULL) const;
        virtual std::string toStringPostfix(const MamdaniExpression* node = NULL) const;

    };

} 
#endif /* FL_MAMDANIANTECEDENT_H */
