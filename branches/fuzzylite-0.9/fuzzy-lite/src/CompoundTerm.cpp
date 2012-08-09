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
 * CompoundTerm.cpp
 *
 *  Created on: Dec 13, 2009
 *      Author: jcrada
 */

#include <fuzzylite/CompoundTerm.h>

namespace fl {

	CompoundTerm::CompoundTerm() :
		LinguisticTerm() {

	}

	CompoundTerm::CompoundTerm(const std::string& name, flScalar minimum,
		flScalar maximum) :
		LinguisticTerm(name, minimum, maximum) {

	}

	CompoundTerm::CompoundTerm(const FuzzyOperator& fuzzy_op, const std::string& name,
		flScalar minimum, flScalar maximum) :
		LinguisticTerm(fuzzy_op, name, minimum, maximum) {

	}

	CompoundTerm::~CompoundTerm() {

	}

	void CompoundTerm::addTerm(const LinguisticTerm& term) {
		_terms.push_back(term.clone());
		if (term.minimum() < minimum() || isinf(minimum())){
			setMinimum(term.minimum());
		}
		if (term.maximum() > maximum() || isinf(maximum())){
			setMaximum(term.maximum());
		}
	}

	void CompoundTerm::clear() {
		for (size_t i = 0 ; i < _terms.size() ; ++i){
			delete _terms[i];
		}
		_terms.clear();
		setMinimum(-INFINITY);
		setMaximum(INFINITY);
	}

	CompoundTerm* CompoundTerm::clone() const{
		//TODO: ¿Deep Copy linguistic variables?
		return new CompoundTerm(*this);
	}

	flScalar CompoundTerm::membership(flScalar crisp) const {
		flScalar max = flScalar(0);
		for (size_t i = 0 ; i < _terms.size() ; ++i){
			max = fuzzyOperator().accumulate(max, _terms[i]->membership(crisp));
		}
		return fuzzyOperator().modulate(modulation(), max);
	}

	LinguisticTerm::eMembershipFunction CompoundTerm::type() const{
		return MF_COMPOUND;
	}

	std::string CompoundTerm::toString() const {
		std::stringstream ss;
		ss << name() << "={ ";
		for (size_t i = 0 ; i < _terms.size() ; ++i){
			ss << _terms[i]->toString();
		}
		ss << "}";
		return ss.str();
	}

} // namespace fuzzy_lite
