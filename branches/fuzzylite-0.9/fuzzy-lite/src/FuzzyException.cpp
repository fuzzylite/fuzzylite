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
#include <sstream>

#include <fuzzylite/FuzzyException.h>
#include <fuzzylite/defs.h>

namespace fl {

	FuzzyException::FuzzyException(const std::string& file, int line,
		const std::string& function) :
		exception(), _message(""), _previous_exception(NULL) {
		setLocation(file, line, function);
	}

	FuzzyException::FuzzyException(const std::string& file, int line,
		const std::string& function, const std::string& message) :
		exception(), _message(message), _previous_exception(NULL) {
		setLocation(file, line, function);
	}

	FuzzyException::FuzzyException(const std::string& file, int line,
		const std::string& function, const std::string& message,
		const FuzzyException& previous_exception) :
		exception(), _message(message), _previous_exception(&previous_exception) {
		setLocation(file, line, function);
	}

	FuzzyException::~FuzzyException() throw () {
	}

	std::string FuzzyException::getName() const throw () {
		return "General Error";
	}

	std::string FuzzyException::getMessage() const throw () {
		return this->_message;
	}

	void FuzzyException::setMessage(const std::string& message) throw () {
		this->_message = message;
	}

	std::string FuzzyException::getLocation() const throw () {
		return this->_location;
	}

	void FuzzyException::setLocation(const std::string& file, int line,
		const std::string& function) throw () {
		std::stringstream ss;
		ss << file + "[" << line << "]::" + function + "()";
		this->_location = ss.str();
	}

	const FuzzyException* FuzzyException::getPreviousException() const throw () {
		return this->_previous_exception;
	}

	void FuzzyException::setPreviousException(const FuzzyException& exception) throw () {
		this->_previous_exception = &exception;
	}

	const char* FuzzyException::what() const throw () {
		return toString().c_str();
	}

	std::string FuzzyException::toString() const throw () {
		return getName() + ": " + getMessage() + "\n\tIN " + getLocation();
	}

	std::string FuzzyException::getStackTrace() const throw () {
		std::string result;
		const FuzzyException* stack = this;
		do {
			result += stack->toString() + "\n";
			stack = stack->getPreviousException();
		} while (stack);
		return result;
	}

	void FuzzyException::Assert(const std::string& file, int line,
		const std::string& function, bool assert, const std::string message) {
		if (!assert){
			throw FuzzyException(file,line,function,message);
		}
	}

	void FuzzyException::main(int argc, char** argv) {
		std::string f(__FILE__);

		FL_LOG( f );
		//        FuzzyException e(FL_AT);
		//        e.addException(FL_AT, FE_UNKNOWN_ERROR, "Nada de esto fue un error");
		//        e.addException(FL_AT, FE_UNKNOWN_ERROR, "Nada de esto fue un error 2");
		//        e.addException(FL_AT, FE_UNKNOWN_ERROR, "Nada de esto fue un error 3");
		//        FL_LOG << "what?: " << e.what() << std::endl;
		//        FL_LOG << "what?: " << e.getLastException().c_str() << std::endl;
		//        FL_LOG << "ps:" + e.getStackTrace() << std::endl;
		//        while (true) {
		//            e.what();
		//        }

	}

}
