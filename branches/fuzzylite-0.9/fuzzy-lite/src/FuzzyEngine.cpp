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
#include <fuzzylite/FuzzyEngine.h>

#include <fuzzylite/MamdaniRule.h>
#include <fuzzylite/FuzzyOperator.h>
#include <fuzzylite/FuzzyExceptions.h>
#include <fuzzylite/defs.h>
#include <fuzzylite/StringOperator.h>

#include <fuzzylite/TriangularTerm.h>
#include <fuzzylite/ShoulderTerm.h>

namespace fl {

	FuzzyEngine::FuzzyEngine() :
		_fuzzy_op(&FuzzyOperator::DefaultFuzzyOperator()) {

	}

	FuzzyEngine::FuzzyEngine(FuzzyOperator& fuzzy_operator) :
		_fuzzy_op(&fuzzy_operator) {

	}

	FuzzyEngine::~FuzzyEngine() {

	}

	void FuzzyEngine::process() {
		for (int i = 0; i < numberOfOutputLVars(); ++i) {
			outputLVar(i)->output().clear();
		}
		for (int i = 0; i < numberOfRules(); ++i) {
			rule(i)->fire(rule(i)->evaluate());
		}
	}

	void FuzzyEngine::softReset() {
		for (int i = numberOfRules() - 1; i >= 0; --i) {
			removeRule(i);
		}
		for (int i = numberOfInputLVars() - 1; i >= 0; --i) {
			removeInputLVar(i);
		}
		for (int i = numberOfOutputLVars() - 1; i >= 0; --i) {
			removeOutputLVar(i);
		}
		for (int i = numberOfHedges() - 1; i >= 0; --i) {
			removeHedge(i);
		}
	}

	void FuzzyEngine::hardReset() {
		for (int i = numberOfRules() - 1; i >= 0; --i) {
			deleteRule(i);
		}
		for (int i = numberOfInputLVars() - 1; i >= 0; --i) {
			deleteInputLVar(i);
		}
		for (int i = numberOfOutputLVars() - 1; i >= 0; --i) {
			deleteOutputLVar(i);
		}
		for (int i = numberOfHedges() - 1; i >= 0; --i) {
			deleteHedge(i);
		}
	}

	void FuzzyEngine::addInputLVar(InputLVar& lvar) {
		_input_lvars.push_back(&lvar);
	}

	InputLVar* FuzzyEngine::inputLVar(int index) const throw (OutOfRangeException) {
		OutOfRangeException::CheckArray(FL_AT,index, numberOfInputLVars()) ;
		return _input_lvars[index];
	}

	InputLVar* FuzzyEngine::inputLVar(const std::string& name) const {
		for (size_t i = 0; i < _input_lvars.size(); ++i) {
			if (_input_lvars[i]->name() == name) {
				return _input_lvars[i];
			}
		}
		return NULL;
	}

	InputLVar* FuzzyEngine::removeInputLVar(int index) throw (OutOfRangeException) {
		OutOfRangeException::CheckArray(FL_AT,index, numberOfInputLVars()) ;
		InputLVar* result = inputLVar(index);
		_input_lvars.erase(_input_lvars.begin() + index);
		return result;
	}

	InputLVar* FuzzyEngine::removeInputLVar(const std::string& name)
		throw (InvalidArgumentException) {
		int index = indexOfInputLVar(name);
		if (index == -1) {
			throw InvalidArgumentException(FL_AT, "Input variable <" + name +"> not registered in fuzzy engine");
		}
		return removeInputLVar(index);
	}

	void FuzzyEngine::deleteInputLVar(int index) throw (OutOfRangeException) {
		delete removeInputLVar(index);
	}

	void FuzzyEngine::deleteInputLVar(const std::string& name) throw (InvalidArgumentException)  {
		delete removeInputLVar(name);
	}

	int FuzzyEngine::indexOfInputLVar(const std::string& name) const {
		for (size_t i = 0; i < _input_lvars.size(); ++i) {
			if (_input_lvars[i]->name() == name) {
				return i;
			}
		}
		return -1;
	}

	int FuzzyEngine::numberOfInputLVars() const {
		return _input_lvars.size();
	}

	void FuzzyEngine::addOutputLVar(OutputLVar& lvar) {
		_output_lvars.push_back(&lvar);
	}

	OutputLVar* FuzzyEngine::outputLVar(int index) const throw (OutOfRangeException) {
		OutOfRangeException::CheckArray(FL_AT, index, numberOfOutputLVars());
		return _output_lvars[index];
	}

	OutputLVar* FuzzyEngine::outputLVar(const std::string& name) const {
		for (size_t i = 0; i < _output_lvars.size(); ++i) {
			if (_output_lvars[i]->name() == name) {
				return _output_lvars[i];
			}
		}
		return NULL;
	}

	OutputLVar* FuzzyEngine::removeOutputLVar(int index) throw (OutOfRangeException) {
		OutOfRangeException::CheckArray(FL_AT,index,numberOfOutputLVars());
		OutputLVar* result = outputLVar(index);
		_output_lvars.erase(_output_lvars.begin() + index);
		return result;
	}

	OutputLVar* FuzzyEngine::removeOutputLVar(const std::string& name) throw (InvalidArgumentException) {
		int index = indexOfOutputLVar(name);
		if (index == -1) {
			throw InvalidArgumentException(FL_AT, "Input variable <" + name + "> not registered in fuzzy engine");
		}
		return removeOutputLVar(index);
	}


	void FuzzyEngine::deleteOutputLVar(int index) throw (OutOfRangeException) {
		delete removeOutputLVar(index);
	}

	void FuzzyEngine::deleteOutputLVar(const std::string& name) throw (InvalidArgumentException) {
		delete removeOutputLVar(name);
	}


	int FuzzyEngine::indexOfOutputLVar(const std::string& name) const {
		for (size_t i = 0; i < _output_lvars.size(); ++i) {
			if (_output_lvars[i]->name() == name) {
				return i;
			}
		}
		return -1;
	}

	int FuzzyEngine::numberOfOutputLVars() const {
		return _output_lvars.size();
	}

	void FuzzyEngine::addHedge(Hedge& hedge) {
		_hedges.push_back(&hedge);
	}

	Hedge* FuzzyEngine::hedge(int index) const throw (OutOfRangeException){
		OutOfRangeException::CheckArray(FL_AT,index,numberOfHedges());
		return _hedges[index];
	}

	Hedge* FuzzyEngine::hedge(const std::string& name) const{
		int index = indexOfHedge(name);
		return index == -1 ? NULL : hedge(index);
	}

	Hedge* FuzzyEngine::removeHedge(int index) throw (OutOfRangeException){
		OutOfRangeException::CheckArray(FL_AT,index,numberOfHedges());
		Hedge* result = _hedges[index];
		_hedges.erase(_hedges.begin() + index);
		return result;
	}

	Hedge* FuzzyEngine::removeHedge(const std::string& name) throw (InvalidArgumentException){
		int index = indexOfHedge(name);
		if (index == -1){
			throw InvalidArgumentException(FL_AT, "Hedge <" + name + "> not registered in fuzzy engine");
		}
		return removeHedge(index);
	}

	void FuzzyEngine::deleteHedge(int index) throw (OutOfRangeException){
		delete removeHedge(index);
	}

	void FuzzyEngine::deleteHedge(const std::string& name) throw (InvalidArgumentException){
		delete removeHedge(name);
	}

	int FuzzyEngine::indexOfHedge(const std::string& name) const{
		for (int i = 0; i < numberOfHedges(); ++i) {
			if (hedge(i)->name() == name) {
				return i;
			}
		}
		return -1;
	}

	int FuzzyEngine::numberOfHedges() const{
		return _hedges.size();
	}

	void FuzzyEngine::setFuzzyOperator(FuzzyOperator& fuzzy_operator) {
		this->_fuzzy_op = &fuzzy_operator;
	}

	FL_INLINE FuzzyOperator& FuzzyEngine::fuzzyOperator() const {
		return *this->_fuzzy_op;
	}

	//RULES

	void FuzzyEngine::addRule(FuzzyRule& rule) {
		_rules.push_back(&rule);
	}

	FuzzyRule* FuzzyEngine::rule(int index) const throw (OutOfRangeException) {
		OutOfRangeException::CheckArray(FL_AT, index, numberOfRules());
		return _rules[index];
	}

	FuzzyRule* FuzzyEngine::removeRule(int index) throw (OutOfRangeException) {
		OutOfRangeException::CheckArray(FL_AT,index,numberOfRules());
		FuzzyRule* result = _rules[index];
		_rules.erase(_rules.begin() + index);
		return result;
	}

	void FuzzyEngine::deleteRule(int index) throw (OutOfRangeException){
		delete removeRule(index);
	}

	std::vector<FuzzyRule*> FuzzyEngine::removeAllRules() {
		std::vector<FuzzyRule*> result = _rules;
		_rules.clear();
		return result;
	}

	flScalar FuzzyEngine::evaluateRule(FuzzyRule& rule) {
		return rule.evaluate();
	}

	int FuzzyEngine::numberOfRules() const {
		return _rules.size();
	}

	FL_INLINE void FuzzyEngine::setInput(const std::string& input_lvar, flScalar value) throw (InvalidArgumentException) {
		InputLVar* input = inputLVar(input_lvar);
		if (!input) {
			throw InvalidArgumentException(FL_AT, "Input variable <" + input_lvar + "> not registered in fuzzy engine");
		}
		input->setInput(value);
	}

	FL_INLINE flScalar FuzzyEngine::output(const std::string& output_lvar) const throw (InvalidArgumentException) {
		OutputLVar* output = outputLVar(output_lvar);
		if (!output) {
			throw InvalidArgumentException(FL_AT, "Output variable <" + output_lvar + "> not registered in fuzzy engine");
		}
		return output->output().defuzzify();
	}

	std::string FuzzyEngine::fuzzyOutput(const std::string& output_lvar) const throw (InvalidArgumentException) {
		flScalar defuzzified = output(output_lvar);
		OutputLVar* output = outputLVar(output_lvar);
		if (!output) {
			throw InvalidArgumentException(FL_AT, "Output variable <" + output_lvar + "> not registered in fuzzy engine");
		}
		return output->fuzzify(defuzzified);
	}


	void FuzzyEngine::main(int argc, char** argv) {
		FuzzyOperator* op = &FuzzyOperator::DefaultFuzzyOperator();
		FuzzyEngine engine(*op);

		engine.addHedge(*new fl::HedgeNot);
		engine.addHedge(*new fl::HedgeSomewhat);
		engine.addHedge(*new fl::HedgeVery);

		fl::InputLVar* energy = new fl::InputLVar("Energy");
		energy->addTerm(*new fl::ShoulderTerm("LOW", 0.25, 0.5, true));
		energy->addTerm(*new fl::TriangularTerm("MEDIUM", 0.25, 0.75));
		energy->addTerm(*new fl::ShoulderTerm("HIGH", 0.50, 0.75, false));
		engine.addInputLVar(*energy);

		fl::OutputLVar* health = new fl::OutputLVar("Health");
		health->addTerm(*new fl::TriangularTerm("BAD", 0.0, 0.50));
		health->addTerm(*new fl::TriangularTerm("REGULAR", 0.25, 0.75));
		health->addTerm(*new fl::TriangularTerm("GOOD", 0.50, 1.00));
		engine.addOutputLVar(*health);

		fl::MamdaniRule* rule1 = new fl::MamdaniRule();
		fl::MamdaniRule* rule2 = new fl::MamdaniRule();
		fl::MamdaniRule* rule3 = new fl::MamdaniRule();
		rule1->parse("if Energy is LOW then Health is BAD", engine);
		rule2->parse("if Energy is MEDIUM then Health is REGULAR", engine);
		rule3->parse("if Energy is HIGH then Health is GOOD", engine);

		engine.addRule(*rule1);
		engine.addRule(*rule2);
		engine.addRule(*rule3);

		for (fl::flScalar in = 0.0; in < 1.1; in += 0.1){
			energy->setInput(in);
			engine.process();
			fl::flScalar out = health->output().defuzzify();
			FL_LOG("Energy=" << in);
			FL_LOG("Energy is " << energy->fuzzify(in));
			FL_LOG("Health=" << out);
			FL_LOG("Health is " << health->fuzzify(out));
			FL_LOG("--");
		}
	}

}
