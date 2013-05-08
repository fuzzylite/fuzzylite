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
 * FuzzyRule.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_FUZZYRULE_H
#define FL_FUZZYRULE_H

#include "fl/fuzzylite.h"

#include "fl/rule/Rule.h"

//Needed here for covariant return
#include "fl/rule/FuzzyAntecedent.h"
#include "fl/rule/FuzzyConsequent.h"

#include <string>

namespace fl {
    class FuzzyAntecedent;
    class FuzzyConsequent;
    class Engine;
    
    class FL_EXPORT FuzzyRule : public Rule {
    protected:
        FuzzyAntecedent* _antecedent;
        FuzzyConsequent* _consequent;
    public:
        FuzzyRule();
        virtual ~FuzzyRule();

        virtual void setAntecedent(FuzzyAntecedent* antecedent);
        virtual FuzzyAntecedent* getAntecedent() const;

        virtual void setConsequent(FuzzyConsequent* consequent);
        virtual FuzzyConsequent* getConsequent() const;

        static FuzzyRule* parse(const std::string& rule, const Engine* engine);
    };

}
#endif /* FL_FUZZYRULE_H */
