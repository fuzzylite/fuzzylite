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
 * FuzzyConsequent.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_FUZZYCONSEQUENT_H
#define FL_FUZZYCONSEQUENT_H

#include "fl/rule/Consequent.h"

#include <vector>

namespace fl {
    class Engine;
    class FuzzyProposition;

    class FL_EXPORT FuzzyConsequent : public Consequent {
    protected:
        std::vector<FuzzyProposition*> _conclusions;

    public:
        FuzzyConsequent();
        virtual ~FuzzyConsequent();
        
        virtual std::vector<FuzzyProposition*> conclusions() const;
        
        virtual void load(const std::string& consequent, const Engine* engine);

        virtual void fire(scalar strength, const TNorm* activation);

        virtual std::string toString() const;

    };

} 
#endif /* FL_FUZZYCONSEQUENT_H */
