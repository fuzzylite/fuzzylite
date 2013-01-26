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

    std::string Exception::btCallStack(const int maxCalls) {
		std::ostringstream backtrace;
		#ifdef FL_UNIX
        int bufferSize = maxCalls;
        void* buffer[bufferSize];
        int backtraceSize = backtrace(buffer, bufferSize);
        char **btSymbols = backtrace_symbols(buffer, backtraceSize);
        if (btSymbols == NULL) {
            backtrace << "[backtrace error] no symbols could be retrieved";
        } else {
			if (backtraceSize == 0) backtrace << "[backtrace is empty]";
            for (int i = 0; i < backtraceSize; ++i) {
                backtrace << btSymbols[i] << "\n";
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

		int backtraceSize  = CaptureStackBackTrace( 0, bufferSize, buffer, NULL);
		SYMBOL_INFO* btSymbol  = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
		btSymbol->MaxNameLen   = 255;
		btSymbol->SizeOfStruct = sizeof( SYMBOL_INFO );

		if (not btSymbol){
			backtrace << "[backtrace error] no symbols could be retrieved";
		}else{
		if (backtraceSize == 0) backtrace << "[backtrace is empty]";
     for(int i = 0; i < backtraceSize; ++i )     {
         SymFromAddr(GetCurrentProcess(), (DWORD64)( buffer[ i ] ), 0, btSymbol);
         backtrace << (backtraceSize - i - 1) << ": " <<
					btSymbol->Name << "- 0x" << btSymbol->Address << "\n";
     }
}
     free( btSymbol );
		#else
		backtrace << "[backtrace error] backtrace not implemented for your operating system";
		#endif
		return backtrace.str();
    }
    //execinfo

    void Exception::signalHandler(int signal) {
        std::ostringstream ex;
        ex << "[caught signal " << signal << "] backtrace:\n";
        ex << fl::Exception::btCallStack();
        throw fl::Exception(ex.str(), FL_AT);
    }

}
