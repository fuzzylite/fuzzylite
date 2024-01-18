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

#ifndef FL_EXCEPTION_H
#define FL_EXCEPTION_H

#include "fl/fuzzylite.h"

#include <exception>
#include <string>
#include <vector>

namespace fl {

    /**

      The Exception class is the only type of exception that is utilized
      throughout the library. If the library is built with the compiling flag
      `-DFL_BACKTRACE=ON`, the method Exception::btCallStack() will provide a
      stack trace when an exception is thrown. Please, have in mind that
      enabling the stack trace requires the external library `dbghelp` in
      the Windows platform, which is generally available in the operating
      system.

      @author Juan Rada-Vilela, Ph.D.
      @since 4.0

     */

#ifdef FL_WINDOWS
    //Disable warning for dllexport of std::exception in Windows
#pragma warning (push)
#pragma warning (disable:4275)
#endif

    class FL_API Exception : public std::exception {
#ifdef FL_WINDOWS
#pragma warning (pop)
#endif
    private:
        std::string _what;
    public:
        explicit Exception(const std::string& what);
        /**
          Constructor to be used in conjunction with macro `FL_AT`
          @param what is the message of the exception
          @param file is the name of the file where the exception occurred
          @param line is the line number in the file where the exception occurred
          @param function is the name of the function where the exception occurred
         */
        explicit Exception(const std::string& what, const std::string& file, int line,
                const std::string& function);
        virtual ~Exception() FL_INOEXCEPT FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Exception)

        /**
          Sets the message of the `std::exception`
          @param what is the message of the `std::exception`
         */
        virtual void setWhat(const std::string& what);
        /**
          Gets the message of the `std::exception`
          @return the message of the `std::exception`
         */
        virtual std::string getWhat() const;
        /**
          Gets the message of the `std::exception`
          @return the message of the `std::exception`
         */
        virtual const char* what() const FL_INOEXCEPT FL_IOVERRIDE;

        /**
          Appends a message to the exception
          @param whatElse is a message to the exception
         */
        virtual void append(const std::string& whatElse);
        /**
          Appends an error trace to the exception. The method can be called
          utilizing the macro `FL_AT`
          @param file is the name of the file where the exception occurred
          @param line is the line number in the file where the exception occurred
          @param function is the name of the function where the exception occurred
         */
        virtual void append(const std::string& file, int line, const std::string& function);
        /**
          Appends an error trace with a message to the exception. The method can be called
          utilizing the macro `FL_AT`
          @param whatElse is further information about the exception
          @param file is the name of the file where the exception occurred
          @param line is the line number in the file where the exception occurred
          @param function is the name of the function where the exception occurred
         */
        virtual void append(const std::string& whatElse,
                const std::string& file, int line, const std::string& function);

        /**
          Returns the stack trace (if enabled)
          @return the stack trace (if enabled)
         */
        static std::string btCallStack();

        /**
          Provides a signal handler to catch signals
          @param signal is the code of the signal
         */
        static void signalHandler(int signal);

        /**
          Converts a given signal into an Exception (does not work very well on Windows)
          @param signal is the code of the signal
         */
        static void convertToException(int signal);

        /**
          Provides a handler for `terminate` and `unexpected` signals
         */
        static void terminate();
        /**
          Logs the exception to console and proceeds the regular execution of the library
          @param exception is the exception thrown
         */
        static void catchException(const std::exception& exception);


    };
}
#endif /* FL_EXCEPTION_H */
