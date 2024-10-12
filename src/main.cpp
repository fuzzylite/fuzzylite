/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include <csignal>
#include <fstream>

#include "fuzzylite/Headers.h"

int main(int argc, const char* argv[]) {
    std::set_terminate(fl::Exception::terminate);
    std::signal(SIGSEGV, fl::Exception::signalHandler);
    std::signal(SIGABRT, fl::Exception::signalHandler);
    std::signal(SIGILL, fl::Exception::signalHandler);
    std::signal(SIGSEGV, fl::Exception::signalHandler);
    std::signal(SIGFPE, fl::Exception::signalHandler);
#ifdef FL_UNIX
    std::signal(SIGBUS, fl::Exception::signalHandler);
    std::signal(SIGPIPE, fl::Exception::signalHandler);
#endif
    fl::fuzzylite::setDebugging(false);

    try {
        fl::Console::main(argc, argv);
    } catch (std::exception& ex) {
        std::cout << ex.what() << "\nBACKTRACE:\n" << fl::Exception::btCallStack() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
