/*   Copyright 2013 Juan Rada-Vilela

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
 * Exception.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "fl/Exception.h"


#ifdef FL_BACKTRACE_OFF
//do nothing
#else
#ifdef FL_UNIX
#include <execinfo.h>

#elif defined FL_WINDOWS
#include <windows.h>
#include <winbase.h>
#include <dbghelp.h>
#endif

#endif

#include <stdlib.h>


namespace fl {

    Exception::Exception(const std::string& what, const std::string& file, int line,
            const std::string& function)
    : std::exception(), _what(what + "\n") {
        append(file, line, function);
        FL_DBG(this->what());
    }

    Exception::~Exception() throw () {
    }

    void Exception::setWhat(const std::string& what) {
        this->_what = what;
    }

    std::string Exception::getWhat() const {
        return this->_what;
    }

    const char* Exception::what() const throw () {
        return this->_what.c_str();
    }

    void Exception::append(const std::string& whatElse) {
        this->_what += whatElse + "\n";
    }

    void Exception::append(const std::string& file, int line, const std::string& function) {
        std::ostringstream ss;
        ss << "{" << file << "::" << function << "() [line:" << line << "]}";
        _what += "\n" + ss.str();
    }

    void Exception::append(const std::string& whatElse,
            const std::string& file, int line, const std::string& function) {
        append(whatElse);
        append(file, line, function);
    }

    std::string Exception::btCallStack(const int maxCalls) {
#ifdef FL_BACKTRACE_OFF
        (void) maxCalls;
        return "[backtrace missing] fuzzylite was built with option -DFL_BACKTRACE_OFF";
#elif defined FL_UNIX
        std::ostringstream btStream;
        int bufferSize = maxCalls;
        void* buffer[bufferSize];
        int backtraceSize = backtrace(buffer, bufferSize);
        char **btSymbols = backtrace_symbols(buffer, backtraceSize);
        if (btSymbols == NULL) {
            btStream << "[backtrace error] no symbols could be retrieved";
        } else {
            if (backtraceSize == 0) btStream << "[backtrace is empty]";
            for (int i = 0; i < backtraceSize; ++i) {
                btStream << btSymbols[i] << "\n";
            }
        }
        free(btSymbols);
        return btStream.str();


#elif defined FL_WINDOWS
        std::ostringstream btStream;
        (void) maxCalls; //Can't allocate an with non-constant size in Windows
        const int bufferSize = 30;
        void* buffer[bufferSize];
        SymInitialize(GetCurrentProcess(), NULL, TRUE);

        int backtraceSize = CaptureStackBackTrace(0, bufferSize, buffer, NULL);
        SYMBOL_INFO* btSymbol = (SYMBOL_INFO *) calloc(sizeof ( SYMBOL_INFO) + 256 * sizeof ( char), 1);
        btSymbol->MaxNameLen = 255;
        btSymbol->SizeOfStruct = sizeof ( SYMBOL_INFO);

        if (not btSymbol) {
            btStream << "[backtrace error] no symbols could be retrieved";
        } else {
            if (backtraceSize == 0) btStream << "[backtrace is empty]";
            for (int i = 0; i < backtraceSize; ++i) {
                SymFromAddr(GetCurrentProcess(), (DWORD64) (buffer[ i ]), 0, btSymbol);
                btStream << (backtraceSize - i - 1) << ": " <<
                        btSymbol->Name << " at 0x" << btSymbol->Address << "\n";
            }
        }
        free(btSymbol);
        return btStream.str();
#else
        return "[backtrace missing] supported only in Unix and Windows platforms";
#endif
    }
    //execinfo

    void Exception::signalHandler(int signal) {
        std::ostringstream ex;
        ex << "[caught signal " << signal << "] backtrace:\n";
        ex << fl::Exception::btCallStack();
        throw fl::Exception(ex.str(), FL_AT);
    }

    void Exception::terminate() {
        std::string message = "[unexpected exception] backtrace:\n"
                + fl::Exception::btCallStack(50);
        FL_LOGP(message);
        exit(EXIT_FAILURE);
    }

}
