/*
 Copyright © 2010-2015 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/Headers.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <signal.h>
#include <typeinfo>

using namespace fl;

/*
#ifdef FL_WINDOWS
#include <windows.h>
BOOL WINAPI flSignalHandler(DWORD dwCtrlType)
{
  FL_LOG("Signal: " << dwCtrlType);
  if (CTRL_C_EVENT == dwCtrlType){
  }
  //return FALSE;
  return TRUE;
}
#endif
 */

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;
    std::set_terminate(fl::Exception::terminate);
    std::set_unexpected(fl::Exception::terminate);
    signal(SIGSEGV, fl::Exception::signalHandler);
    signal(SIGABRT, fl::Exception::signalHandler);
    signal(SIGILL, fl::Exception::signalHandler);
    signal(SIGSEGV, fl::Exception::signalHandler);
    signal(SIGFPE, fl::Exception::signalHandler);
#ifdef FL_UNIX
    signal(SIGBUS, fl::Exception::signalHandler);
    signal(SIGPIPE, fl::Exception::signalHandler);
#endif
#ifdef FL_WINDOWS
    //SetConsoleCtrlHandler(flSignalHandler, TRUE);
#endif
    fuzzylite::setDebug(false);
    try {
        Console::main(argc, argv);
    } catch (std::exception& ex) {
        std::cout << ex.what() << "\nBACKTRACE:\n" <<
                fl::Exception::btCallStack() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}



