/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * main.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/Headers.h"

#include <typeinfo>
#include <iomanip>
#include <cstdlib>
#include <signal.h>
#include <fstream>

using namespace fl;

/*
void baz(){
    int *x = (int*) - 1; // make a bad pointer
    FL_LOG("%d\n" <<  *x); // causes segfault
}

void bar(){
    baz();
}

void foo(){
    bar();
}
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

    try {
        return Console::main(argc, argv);
    } catch (std::exception& ex) {
        FL_LOG(ex.what());
        FL_LOG("BACKTRACE:\n" << fl::Exception::btCallStack());
        return EXIT_FAILURE;
    }
}

