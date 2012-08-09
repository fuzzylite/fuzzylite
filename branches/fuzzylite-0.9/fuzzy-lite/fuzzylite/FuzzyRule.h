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
 * File:   FuzzyRule.h
 * Author: jcrada
 *
 * Created on November 5, 2009, 11:11 AM
 */

#ifndef _FUZZYRULE_H
#define	_FUZZYRULE_H

#include <fuzzylite/FuzzyOperator.h>
#include <fuzzylite/FuzzyEngine.h>

#include <string>

namespace fl {
	class FuzzyEngine; //forward declaration

	class FuzzyRule {
	public:
		static const std::string FR_IF;
		static const std::string FR_IS;
		static const std::string FR_THEN;
		static const std::string FR_AND;
		static const std::string FR_OR;
		static const std::string FR_WITH;

	public:
		virtual ~FuzzyRule() {
		}

		virtual bool isValid() const = 0;
		virtual flScalar evaluate() const = 0;
		virtual void fire(flScalar degree) = 0;

		virtual void parse(const std::string& rule, const FuzzyEngine& engine) = 0;
		virtual std::string toString() const = 0;
	};


}

#endif	/* _FUZZYRULE_H */

