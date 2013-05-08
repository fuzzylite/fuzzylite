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
 * FuzzyAntecedent.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_FUZZYANTECEDENT_H
#define FL_FUZZYANTECEDENT_H

#include "fl/rule/Antecedent.h"

#include <sstream>
#include <vector>

namespace fl {
    class Engine;
    class TNorm;
    class SNorm;
    class FuzzyExpression;

    class FL_EXPORT FuzzyAntecedent : public Antecedent {
    protected:
        FuzzyExpression* _root;

    public:
        FuzzyAntecedent();
        virtual ~FuzzyAntecedent();

        virtual FuzzyExpression* getRoot() const;

        virtual void load(const std::string& antecedent, const Engine* engine);

        virtual scalar firingStrength(const TNorm* tnorm, const SNorm* snorm,
                const FuzzyExpression* node) const;

        virtual scalar firingStrength(const TNorm* tnorm, const SNorm* snorm) const;

        virtual std::string toString() const;

        virtual std::string toStringPrefix(const FuzzyExpression* node = NULL) const;
        virtual std::string toStringInfix(const FuzzyExpression* node = NULL) const;
        virtual std::string toStringPostfix(const FuzzyExpression* node = NULL) const;

    };

}
#endif /* FL_FUZZYANTECEDENT_H */
