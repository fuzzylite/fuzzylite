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
 * Rule.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_RULE_H
#define FL_RULE_H

#include "fl/fuzzylite.h"



#include <string>

namespace fl {

    class Antecedent;
    class Consequent;
    class TNorm;
    class SNorm;

    class FL_EXPORT Rule {
    protected:
        scalar _weight;
        std::string _unparsedRule;

        virtual void setUnparsedRule(const std::string& unparsedRule);
    public:
        Rule();
        virtual ~Rule();

        virtual Antecedent* getAntecedent() const = 0;

        virtual Consequent* getConsequent() const = 0;

        virtual scalar firingStrength(const TNorm* tnorm, const SNorm* snorm) const;
        virtual void fire(scalar strength, const TNorm* activation) const;
        
        virtual void setWeight(scalar weight);
        virtual scalar getWeight() const;
        
        virtual std::string getUnparsedRule() const;

        virtual std::string toString() const;

        static  std::string FL_IF;
        static  std::string FL_IS;
        static  std::string FL_THEN;
        static  std::string FL_AND;
        static  std::string FL_OR;
        static  std::string FL_WITH;
		
		static std::string ifKeyword();
		static std::string isKeyword();
		static std::string thenKeyword();
		static std::string andKeyword();
		static std::string orKeyword();
		static std::string withKeyword();
		
		
    };
}


#endif /* FL_RULE_H */
