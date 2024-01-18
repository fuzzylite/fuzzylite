/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/Exception.h"


#ifdef FL_BACKTRACE

#ifdef FL_UNIX
#include <execinfo.h>

#elif defined FL_WINDOWS
#include <windows.h>
#include <winbase.h>

#ifndef __MINGW32__
/*Disable warning 8.1\Include\um\dbghelp.h(1544):
warning C4091: 'typedef ': ignored on left of '' when no variable is declared*/
#pragma warning (push)
#pragma warning (disable:4091)
#include <dbghelp.h>
#pragma warning (pop)
#endif

#endif

#endif


#include <csignal>
#include <cstring>

namespace fl {

    Exception::Exception(const std::string& what)
    : std::exception(), _what(what) {
        FL_DBG(this->what());
    }

    Exception::Exception(const std::string& what, const std::string& file, int line,
            const std::string& function)
    : std::exception(), _what(what) {
        append(file, line, function);
        FL_DBG(this->what());
    }

    Exception::~Exception() FL_INOEXCEPT { }

    void Exception::setWhat(const std::string& what) {
        this->_what = what;
    }

    std::string Exception::getWhat() const {
        return this->_what;
    }

    const char* Exception::what() const FL_INOEXCEPT {
        return this->_what.c_str();
    }

    void Exception::append(const std::string& whatElse) {
        this->_what += whatElse + "\n";
    }

    void Exception::append(const std::string& file, int line, const std::string& function) {
        std::ostringstream ss;
        ss << "\n{at " << file << "::" << function << "() [line:" << line << "]}";
        _what += ss.str();
    }

    void Exception::append(const std::string& whatElse,
            const std::string& file, int line, const std::string& function) {
        append(whatElse);
        append(file, line, function);
    }

    std::string Exception::btCallStack() {
#ifndef FL_BACKTRACE
        return "[backtrace disabled] fuzzylite was built without -DFL_BACKTRACE";
#elif defined FL_UNIX
        std::ostringstream btStream;
        const int bufferSize = 30;
        void* buffer[bufferSize];
        int backtraceSize = ::backtrace(buffer, bufferSize);
        char **btSymbols = ::backtrace_symbols(buffer, backtraceSize);
        if (btSymbols == fl::null) {
            btStream << "[backtrace error] no symbols could be retrieved";
        } else {
            if (backtraceSize == 0) {
                btStream << "[backtrace is empty]";
            }
            for (int i = 0; i < backtraceSize; ++i) {
                btStream << btSymbols[i] << "\n";
            }
        }
        ::free(btSymbols);
        return btStream.str();


#elif defined FL_WINDOWS && ! defined __MINGW32__
        std::ostringstream btStream;
        const int bufferSize = 30;
        void* buffer[bufferSize];
        SymInitialize(GetCurrentProcess(), fl::null, TRUE);

        int backtraceSize = CaptureStackBackTrace(0, bufferSize, buffer, fl::null);
        SYMBOL_INFO* btSymbol = (SYMBOL_INFO *) calloc(sizeof ( SYMBOL_INFO) + 256 * sizeof ( char), 1);
        if (not btSymbol) {
            btStream << "[backtrace error] no symbols could be retrieved";
        } else {
            btSymbol->MaxNameLen = 255;
            btSymbol->SizeOfStruct = sizeof ( SYMBOL_INFO);
            if (backtraceSize == 0) {
                btStream << "[backtrace is empty]";
            }
            for (int i = 0; i < backtraceSize; ++i) {
                SymFromAddr(GetCurrentProcess(), (DWORD64) (buffer[ i ]), 0, btSymbol);
                btStream << (backtraceSize - i - 1) << ": " <<
                        btSymbol->Name << " at 0x" << btSymbol->Address << "\n";
            }
        }
        ::free(btSymbol);
        return btStream.str();
#else
        return "[backtrace missing] supported only in Unix and Windows platforms";
#endif
    }

    void Exception::signalHandler(int unixSignal) {
        std::ostringstream ex;
        ex << "[unexpected signal " << unixSignal << "] ";
#ifdef FL_UNIX
        ex << ::strsignal(unixSignal);
#endif
        ex << "\nBACKTRACE:\n" << btCallStack();
        Exception::catchException(Exception(ex.str(), FL_AT));
        ::exit(EXIT_FAILURE);
    }

    void Exception::convertToException(int unixSignal) {
        std::string signalDescription;
#ifdef FL_UNIX
        //Unblock the signal
        sigset_t empty;
        sigemptyset(&empty);
        sigaddset(&empty, unixSignal);
        sigprocmask(SIG_UNBLOCK, &empty, fl::null);
        signalDescription = ::strsignal(unixSignal);
#endif
        std::ostringstream ex;
        ex << "[signal " << unixSignal << "] " << signalDescription << "\n";
        ex << "BACKTRACE:\n" << btCallStack();
        throw Exception(ex.str(), FL_AT);
    }

    void Exception::terminate() {
        Exception::catchException(Exception("[unexpected exception] BACKTRACE:\n" + btCallStack(), FL_AT));
        ::exit(EXIT_FAILURE);
    }

    void Exception::catchException(const std::exception& exception) {
        std::ostringstream ss;
        ss << exception.what();
        std::string backtrace = btCallStack();
        if (not backtrace.empty()) {
            ss << "\n\nBACKTRACE:\n" << backtrace;
        }
        FL_LOG(ss.str());
    }

}
