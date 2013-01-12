/*
 * Exception.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "fl/Exception.h"
#include "fl/definitions.h"

namespace fl {

    Exception::Exception(const std::string& what, const std::string& file, int line,
            const std::string& function, bool log)
    : std::exception(), _what(what) {
        addCall(file, line, function);
        if (log) FL_LOGP(this->what());
    }

    Exception::~Exception() throw () { }

    void Exception::setWhat(const std::string& what) {
        this->_what = what;
    }

    const char* Exception::what() const throw () {
        std::ostringstream ex;
        ex << this->_what;
        for (std::size_t i = 0; i < _details.size(); ++i) {
            ex << "\n" << this->_details.at(i);
        }
        if (_calls.size() > 0) ex << "\n";
        for (std::size_t i = 0; i < _calls.size(); ++i) {
            ex << "\n" << this->_calls.at(i) ;
        }
        return ex.str().c_str();
    }

    /**
     * Operations for std::vector _details
     */
    void Exception::addDetail(const std::string& detail) {
        this->_details.push_back(detail);
    }

    void Exception::addDetail(const std::string& detail,
            const std::string& file, int line, const std::string& function) {
        this->_details.push_back(detail);
        addCall(file,line,function);
    }

    std::string Exception::getDetail(int index) const {
        return this->_details.at(index);
    }

    std::string Exception::removeDetail(int index) {
        std::string result = this->_details.at(index);
        this->_details.erase(this->_details.begin() + index);
        return result;
    }

    int Exception::numberOfDetails() const {
        return this->_details.size();
    }

    const std::vector<std::string>& Exception::details() const {
        return this->_details;
    }

    /**
     * Operations for std::vector _calls
     */
    void Exception::addCall(const std::string& file, int line, const std::string& function) {
        std::ostringstream call;
        call << "{" << file << "::" << function << "() [line:" << line << "]}";
        this->_calls.push_back(call.str());
    }

    std::string Exception::getCall(int index) const {
        return this->_calls.at(index);
    }

    std::string Exception::removeCall(int index) {
        std::string result = this->_calls.at(index);
        this->_calls.erase(this->_calls.begin() + index);
        return result;
    }

    int Exception::numberOfCalls() const {
        return this->_calls.size();
    }

    const std::vector<std::string>& Exception::calls() const {
        return this->_calls;
    }




}
