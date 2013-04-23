/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
