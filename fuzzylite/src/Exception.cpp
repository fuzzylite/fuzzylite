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
    : std::exception(), _what(what){
        std::ostringstream detail;
        detail << "{" << file << "::" << function << "() [line:" << line << "]}";
        appendDetail(detail.str());
        if (log) FL_LOGP(detail.str() << ": " << what);
    }

    Exception::~Exception() throw () { }

    void Exception::setWhat(const std::string& what) {
        this->_what = what;
    }
    
    void Exception::appendDetail(const std::string& detail){
        std::ostringstream ss;
        ss << this->_what << "\n" << detail;
        this->_what = ss.str();
    }
    
    const char* Exception::what() const throw () {
        return this->_what.c_str();
    }

} 
