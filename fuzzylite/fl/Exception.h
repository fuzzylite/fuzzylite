/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_EXCEPTION_H
#define FL_EXCEPTION_H

#include "fl/fuzzylite.h"

#include <exception>
#include <string>
#include <vector>

namespace fl {

    class FL_API Exception : public std::exception {
    protected:
        std::string _what;
    public:
        explicit Exception(const std::string& what);
        Exception(const std::string& what, const std::string& file, int line,
                const std::string& function);
        virtual ~Exception() FL_INOEXCEPT FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Exception)

        virtual void setWhat(const std::string& what);
        virtual std::string getWhat() const;
        virtual const char* what() const FL_INOEXCEPT FL_IOVERRIDE;

        virtual void append(const std::string& whatElse);
        virtual void append(const std::string& file, int line, const std::string& function);
        virtual void append(const std::string& whatElse,
                const std::string& file, int line, const std::string& function);

        static std::string btCallStack();

        static void signalHandler(int signal);
        static void convertToException(int signal);
        static void terminate();
        static void catchException(const std::exception& exception);


    };

}
#endif /* FL_EXCEPTION_H */
