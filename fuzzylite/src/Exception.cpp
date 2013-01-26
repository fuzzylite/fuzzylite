/*
 * Exception.cpp
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#include "fl/Exception.h"
#include "fl/config.h"

#ifdef FL_UNIX
#include <execinfo.h>

#elif defined FL_WINDOWS
#include <windows.h>
#include <winbase.h>
#include <dbghelp.h>
#endif

#include <stdlib.h>


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

    std::string Exception::btCallStack(const int maxCalls) {
        std::ostringstream btStream;
#ifdef FL_UNIX
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
        /**
         *	WINDOWS:
         */
#elif defined FL_WINDOWS
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
                        btSymbol->Name << "- 0x" << btSymbol->Address << "\n";
            }
        }
        free(btSymbol);
#else
        btStream << "[backtrace error] backtrace not implemented for your operating system";
#endif
        return btStream.str();
    }
    //execinfo

    void Exception::signalHandler(int signal) {
        std::ostringstream ex;
        ex << "[caught signal " << signal << "] backtrace:\n";
        ex << fl::Exception::btCallStack();
        throw fl::Exception(ex.str(), FL_AT);
    }

}
