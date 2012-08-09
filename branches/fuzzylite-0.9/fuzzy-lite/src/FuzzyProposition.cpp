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
#include <fuzzylite/FuzzyProposition.h>
#include <fuzzylite/MamdaniRule.h>
#include <fuzzylite/FuzzyExceptions.h>
#include <fuzzylite/StringOperator.h>

#include <sstream>

namespace fl {

	FuzzyProposition::FuzzyProposition() :
		_fuzzy_operator(NULL), _left(NULL), _right(NULL), _operation(O_NONE),
			_input_lvar(NULL), _term(NULL) {
		setFuzzyOperator(FuzzyOperator::DefaultFuzzyOperator());
	}

	FuzzyProposition::FuzzyProposition(const FuzzyOperator& fuzzy_operator) :
		_fuzzy_operator(&fuzzy_operator), _left(NULL), _right(NULL), _operation(O_NONE),
			_input_lvar(NULL), _term(NULL) {
	}

	FuzzyProposition::~FuzzyProposition() {
		if (_left) {
			delete _left;
		}
		if (_right) {
			delete _right;
		}
	}

	void FuzzyProposition::setFuzzyOperator(const FuzzyOperator& fuzzy_operator) {
		this->_fuzzy_operator = &fuzzy_operator;
	}

	const FuzzyOperator& FuzzyProposition::fuzzyOperator() const {
		return *this->_fuzzy_operator;
	}

	void FuzzyProposition::setLeft(FuzzyProposition& proposition) {
		this->_left = &proposition;
	}

	void FuzzyProposition::setRight(FuzzyProposition& proposition) {
		this->_right = &proposition;
	}

	void FuzzyProposition::setOperation(OPERATION operation) {
		this->_operation = operation;
	}

	void FuzzyProposition::setInputLVar(const InputLVar& lvar) {
		this->_input_lvar = &lvar;
	}

	void FuzzyProposition::addHedge(Hedge& hedge) {
		this->_hedges.push_back(&hedge);
	}

	void FuzzyProposition::setTerm(const LinguisticTerm& term) {
		this->_term = &term;
	}

	bool FuzzyProposition::isTerminal() const {
		return this->_operation == O_NONE;
	}

	FL_INLINE flScalar FuzzyProposition::degreeOfTruth() const {
		if (!isTerminal()) {
			if (_left == NULL || _right == NULL) {
				throw NullPointerException(FL_AT,toString()) ;
			}
			switch (_operation) {
			case O_AND:
				return fuzzyOperator().fuzzyAnd(_left->degreeOfTruth(),
					_right->degreeOfTruth());
			case O_OR:
				return fuzzyOperator().fuzzyOr(_left->degreeOfTruth(),
					_right->degreeOfTruth());
			default:
				throw InvalidArgumentException(FL_AT, "Operation " + StringOperator::IntToString(_operation) + " not available");
				}
			}

			flScalar result = _term->membership(_input_lvar->input());
			for (size_t i = 0; i < _hedges.size(); ++i) {
				result = _hedges[i]->hedge(result);
			}
			return result;
		}

		std::string FuzzyProposition::toString() const {
			std::stringstream ss;
			if (isTerminal()) {
				ss << _input_lvar->name() + " " + MamdaniRule::FR_IS + " ";
				for (size_t i = 0; i < _hedges.size(); ++i) {
					ss << _hedges[i]->name() << " ";
				}
				ss << _term->name();
			} else {
				ss << " ( " + (_left ? _left->toString() : "NULL");
				ss << " " + (_operation == O_AND ? MamdaniRule::FR_AND : MamdaniRule::FR_OR);
						ss << " " + (_right ? _right->toString() : "NULL") + " ) ";
					}
					return ss.str();
				}
}
