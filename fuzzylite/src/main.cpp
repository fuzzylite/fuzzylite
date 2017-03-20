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

#include "fl/Headers.h"

#include <fstream>
#include <csignal>

int main(int argc, const char* argv[]) {
    std::set_terminate(fl::Exception::terminate);
    std::set_unexpected(fl::Exception::terminate);
    ::signal(SIGSEGV, fl::Exception::signalHandler);
    ::signal(SIGABRT, fl::Exception::signalHandler);
    ::signal(SIGILL, fl::Exception::signalHandler);
    ::signal(SIGSEGV, fl::Exception::signalHandler);
    ::signal(SIGFPE, fl::Exception::signalHandler);
#ifdef FL_UNIX
    ::signal(SIGBUS, fl::Exception::signalHandler);
    ::signal(SIGPIPE, fl::Exception::signalHandler);
#endif
#ifdef FL_WINDOWS
    //SetConsoleCtrlHandler(flSignalHandler, TRUE);
#endif
    fl::fuzzylite::setDebugging(false);

    try {
        fl::Console::main(argc, argv);
    } catch (std::exception& ex) {
        std::cout << ex.what() << "\nBACKTRACE:\n" <<
                fl::Exception::btCallStack() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}



