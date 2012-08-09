/*   Copyright 2010 Juan Rada-Vilela

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
 * File:   MamdaniRule.h
 * Author: jcrada
 *
 * Created on November 5, 2009, 11:15 AM
 */

#ifndef _MAMDANIRULE_H
#define	_MAMDANIRULE_H

#include <fuzzylite/FuzzyRule.h>
#include <fuzzylite/FuzzyProposition.h>
#include <fuzzylite/FuzzyAction.h>

namespace fl {

    class MamdaniRule : public FuzzyRule {
    private:
        FuzzyProposition* _antecedent;
        std::vector<FuzzyAction*> _consequent;
        std::string _unparsed_rule;
    protected:
        virtual void parseAntecedent(const std::string& postfix_antecedent,
                const FuzzyEngine& engine) throw (RuleParsingException);
        virtual void parseConsequent(const std::string& consequent,
                const FuzzyEngine& engine) throw (RuleParsingException);
    public:

        MamdaniRule();
        virtual ~MamdaniRule();

        virtual flScalar evaluate() const;
        virtual bool isValid() const;
        virtual void fire(flScalar degree);

        virtual void parse(const std::string& rule, const FuzzyEngine& engine) throw (RuleParsingException);

        virtual std::string getUnparsedRule() const;

        virtual std::string toString() const;

        static void Extract(const std::string& rule, std::string& antecedent, std::string& consequent);
        static std::string InfixToPostfix(const std::string& infix);
        static bool Validate(const std::string& rule, const FuzzyEngine& engine);

        static void testComplex();
        static void testSimple();

        static void main(int argc, char** argv);

    };
}

#endif	/* _MAMDANIRULE_H */

