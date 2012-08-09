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
#include <fuzzylite/FuzzyAction.h>

#include <fuzzylite/MamdaniRule.h>

#include <sstream>
namespace fl {

	FuzzyAction::FuzzyAction() :
		_output_lvar(NULL), _term(NULL), _weight(1.0) {

	}

	FuzzyAction::~FuzzyAction() {

	}

	void FuzzyAction::setOutputLVar(OutputLVar* output_lvar) {
		this->_output_lvar = output_lvar;
	}

	OutputLVar* FuzzyAction::outputLVar() const{
		return this->_output_lvar;
	}

	void FuzzyAction::setTerm(const LinguisticTerm& term) {
		this->_term = &term;
	}

	const LinguisticTerm* FuzzyAction::term() const{
		return this->_term;
	}

	void FuzzyAction::setWeight(flScalar weight) {
		this->_weight = weight;
	}

	flScalar FuzzyAction::weight() const {
		return this->_weight;
	}

	void FuzzyAction::addHedge(Hedge& hedge) {
		this->_hedges.push_back(&hedge);
	}

	Hedge* FuzzyAction::hedge(int index) const throw (OutOfRangeException) {
		OutOfRangeException::CheckArray(FL_AT,index, numberOfHedges()) ;
		return _hedges[index];
	}

	int FuzzyAction::numberOfHedges() const {
		return _hedges.size();
	}

	void FuzzyAction::execute(flScalar degree) {
		LinguisticTerm* rule_output = _term->clone();
		flScalar modulation = degree * weight();
		for (int i = 0; i < numberOfHedges(); ++i) {
			modulation = hedge(i)->hedge(modulation);
		}
		rule_output->setModulation(modulation);
//		FL_LOG(rule_output->toString());
		outputLVar()->output().addTerm(*rule_output);
		delete rule_output;
	}

	std::string FuzzyAction::toString() const {
		std::stringstream ss;
		ss << outputLVar()->name() << " " << MamdaniRule::FR_IS;
		for (int i = 0 ; i < numberOfHedges() ; ++i){
			ss << " " << hedge(i)->name();
		}
		ss << " " << term()->name();
		if (weight() < flScalar(1.0)) {
			ss << " WITH " << weight();
		}
		return ss.str();
	}
}
