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
 * File:   FuzzyProposition.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 6:19 PM
 */

#ifndef _FUZZYPROPOSITION_H
#define	_FUZZYPROPOSITION_H

#include <fuzzylite/InputLVar.h>
#include <fuzzylite/Hedge.h>
#include <fuzzylite/FuzzyOperator.h>
#include <fuzzylite/LinguisticTerm.h>

namespace fl {

	class FuzzyProposition {
	public:

		enum OPERATION {
			O_NONE = ' ', O_AND = '&', O_OR = '*',
		};
	private:
		const FuzzyOperator* _fuzzy_operator;
		FuzzyProposition* _left;
		FuzzyProposition* _right;
		OPERATION _operation;
		const InputLVar* _input_lvar; //e.g. power
		std::vector<Hedge*> _hedges; //very
		const LinguisticTerm* _term; //high

	public:
		FuzzyProposition();
		FuzzyProposition(const FuzzyOperator& fuzzy_operator);
		virtual ~FuzzyProposition();

		virtual void setFuzzyOperator(const FuzzyOperator& fuzzy_operator);
		virtual const FuzzyOperator& fuzzyOperator() const;

		virtual void setLeft(FuzzyProposition& proposition);
		virtual void setRight(FuzzyProposition& proposition);
		virtual void setOperation(OPERATION operation);
		virtual void setInputLVar(const InputLVar& lvar);
		virtual void addHedge(Hedge& hedge);
		virtual void setTerm(const LinguisticTerm& term);

		virtual bool isTerminal() const;

		virtual flScalar degreeOfTruth() const;

		virtual std::string toString() const;

	};
}

#endif	/* _FUZZYPROPOSITION_H */

