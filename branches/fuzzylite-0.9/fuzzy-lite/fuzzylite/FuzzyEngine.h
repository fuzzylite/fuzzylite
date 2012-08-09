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
 * File:   FuzzyEngine.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 4:51 PM
 */

#ifndef _FUZZYENGINE_H
#define	_FUZZYENGINE_H

#include <fuzzylite/FuzzyOperator.h>
#include <fuzzylite/InputLVar.h>
#include <fuzzylite/OutputLVar.h>
#include <fuzzylite/Hedge.h>
#include <fuzzylite/FuzzyRule.h>

namespace fl {
	class FuzzyRule; //forward declaration

	class FuzzyEngine {
	private:
		FuzzyOperator* _fuzzy_op;
		std::vector<InputLVar*> _input_lvars;
		std::vector<OutputLVar*> _output_lvars;
		std::vector<Hedge*> _hedges;
		std::vector<FuzzyRule*> _rules;
	public:
		FuzzyEngine();
		FuzzyEngine(FuzzyOperator& fuzzy_operator);
		virtual ~FuzzyEngine();

		virtual void process();

		virtual void softReset();
		virtual void hardReset();

		virtual void addInputLVar(InputLVar& lvar);
		virtual InputLVar* inputLVar(int index) const throw (OutOfRangeException);
		virtual InputLVar* inputLVar(const std::string& name) const;
		virtual InputLVar* removeInputLVar(int index) throw (OutOfRangeException);
		virtual InputLVar* removeInputLVar(const std::string& name)
			throw (InvalidArgumentException);
		virtual void deleteInputLVar(int index) throw (OutOfRangeException);
		virtual void deleteInputLVar(const std::string& name)
			throw (InvalidArgumentException);
		virtual int indexOfInputLVar(const std::string& name) const;
		virtual int numberOfInputLVars() const;

		virtual void addOutputLVar(OutputLVar& lvar);
		virtual OutputLVar* outputLVar(int index) const throw (OutOfRangeException);
		virtual OutputLVar* outputLVar(const std::string& name) const;
		virtual OutputLVar* removeOutputLVar(int index) throw (OutOfRangeException);
		virtual OutputLVar* removeOutputLVar(const std::string& name)
			throw (InvalidArgumentException);
		virtual void deleteOutputLVar(int index) throw (OutOfRangeException);
		virtual void deleteOutputLVar(const std::string& name)
			throw (InvalidArgumentException);
		virtual int indexOfOutputLVar(const std::string& name) const;
		virtual int numberOfOutputLVars() const;

		virtual void addHedge(Hedge& hedge);
		virtual Hedge* hedge(int index) const throw (OutOfRangeException);
		virtual Hedge* hedge(const std::string& name) const;
		virtual Hedge* removeHedge(int index) throw (OutOfRangeException);
		virtual Hedge* removeHedge(const std::string& name)
			throw (InvalidArgumentException);
		virtual void deleteHedge(const std::string& name)
			throw (InvalidArgumentException);
		virtual void deleteHedge(int index) throw (OutOfRangeException);
		virtual int indexOfHedge(const std::string& name) const;
		virtual int numberOfHedges() const;

		virtual void setFuzzyOperator(FuzzyOperator& fuzzy_operator);
		virtual FuzzyOperator& fuzzyOperator() const;

		virtual void addRule(FuzzyRule& rule);
		virtual FuzzyRule* rule(int index) const throw (OutOfRangeException);
		virtual FuzzyRule* removeRule(int index) throw (OutOfRangeException);
		virtual void deleteRule(int index) throw (OutOfRangeException);
		virtual std::vector<FuzzyRule*> removeAllRules();
		virtual flScalar evaluateRule(FuzzyRule& rule);
		virtual int numberOfRules() const;

		virtual void setInput(const std::string& input_lvar, flScalar value)
			throw (InvalidArgumentException);
		virtual flScalar output(const std::string& output_lvar) const
			throw (InvalidArgumentException);

		virtual std::string fuzzyOutput(const std::string& output_lvar) const
			throw (InvalidArgumentException);

		static void main(int argc, char** argv);

	};
}

#endif	/* _FUZZYENGINE_H */

