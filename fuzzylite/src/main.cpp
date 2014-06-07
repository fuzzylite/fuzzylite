// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

/*
 * main.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
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
    signal(SIGSEGV, fl::Exception::convertToException);
    signal(SIGABRT, fl::Exception::convertToException);
    signal(SIGILL, fl::Exception::convertToException);
    signal(SIGSEGV, fl::Exception::convertToException);
    signal(SIGFPE, fl::Exception::convertToException);
#ifdef FL_UNIX
    signal(SIGBUS, fl::Exception::convertToException);
    signal(SIGPIPE, fl::Exception::convertToException);
#endif
#ifdef FL_WINDOWS
    //SetConsoleCtrlHandler(flSignalHandler, TRUE);
#endif
    try {
        fuzzylite::setDebug(false);
        return Console::main(argc, argv);
    } catch (std::exception& ex) {
        FL_LOG(ex.what());
        return EXIT_FAILURE;
    }
}



