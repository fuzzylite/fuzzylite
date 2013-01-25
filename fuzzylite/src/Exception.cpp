/*
 * Exception.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "fl/Exception.h"
#include "fl/config.h"

#include <execinfo.h>

#include <stdio.h>
#include <stdlib.h>
#include <cstring>

//TODO:Get rid of many things here.

namespace fl {

    Exception::Exception(const std::string& what, const std::string& file, int line,
            const std::string& function, bool log)
    : std::exception(), _what(what) {
        append(file, line, function);
        if (log) FL_LOGP(this->what());
    }

    Exception::~Exception() throw () { }

    void Exception::setWhat(const std::string& what) {
        this->_what = what;
    }

    std::string Exception::getWhat() const {
        return this->_what;
    }

    const char* Exception::what() const throw () {
        return this->_what.c_str();
    }

    void Exception::append(const std::string& whatMore) {
        this->_what += "\n" + whatMore;
    }

    void Exception::append(const std::string& file, int line, const std::string& function) {
        std::ostringstream ss;
        ss << "{" << file << "::" << function << "() [line:" << line << "]}";
        _what += "\n" + ss.str();
    }

    void Exception::append(const std::string& whatMore,
            const std::string& file, int line, const std::string& function) {
        append(whatMore);
        append(file, line, function);
    }

    std::string Exception::btCallStack(int maxCalls) {
        int bufferSize = maxCalls;
        void* buffer[bufferSize];

        int backtraceSize = backtrace(buffer, bufferSize);

        /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
           would produce similar output to the following: */

        std::ostringstream backtrace;
        char **btSymbols = backtrace_symbols(buffer, backtraceSize);
        if (btSymbols == NULL) {
            backtrace << "[backtrace error] no symbols could be retrieved";
        } else {
            for (int i = 0; i < backtraceSize; ++i) {
                backtrace << btSymbols[i] << "\n";
            }
        }
        free(btSymbols);
        return backtrace.str();
    }
    //execinfo

    void Exception::signalHandler(int signal) {
        std::ostringstream ex;
        ex << "[caught signal " << signal << "] backtrace:\n";
        ex << btCallStack();
        throw fl::Exception(ex.str(), FL_AT);
    }

}
