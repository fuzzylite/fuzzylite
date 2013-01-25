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
            ex << "\n" << this->_calls.at(i);
        }
        return ex.str().c_str();
    }

    std::string Exception::btCallStack()  {
        int bufferSize = 100;
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
                backtrace << std::string(btSymbols[i]) + "\n";
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

    ////libunwind
    //    void Exception::signalHandler(int signal) {
    //        char name[256];
    //        unw_cursor_t cursor;
    //        unw_context_t uc;
    //        unw_word_t ip, sp, offp;
    //
    //        unw_getcontext(&uc);
    //        unw_init_local(&cursor, &uc);
    //
    //        while (unw_step(&cursor) > 0) {
    //            char file[256];
    //            int line = 0;
    //
    //            name[0] = '\0';
    //            unw_get_proc_name(&cursor, name, 256, &offp);
    //            unw_get_reg(&cursor, UNW_REG_IP, &ip);
    //            unw_get_reg(&cursor, UNW_REG_SP, &sp);
    //
    //            //printf ("%s ip = %lx, sp = %lx\n", name, (long) ip, (long) sp);
    //            getFileAndLine((long) ip, file, 256, &line);
    //            printf("%s in file %s line %d\n", name, file, line);
    //        }
    //    }



    /**
     * Operations for std::vector _details
     */
    void Exception::addDetail(const std::string& detail) {
        this->_details.push_back(detail);
    }

    void Exception::addDetail(const std::string& detail,
            const std::string& file, int line, const std::string& function) {
        this->_details.push_back(detail);
        addCall(file, line, function);
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
