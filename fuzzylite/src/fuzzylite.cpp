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
 * fuzzylite.cpp
 *
 *  Created on 1 February 2013, 10:47 AM
 *      Author: jcrada
 */

#include "fl/fuzzylite.h"

namespace fl {

    int fuzzylite::_decimals = 3;
    scalar fuzzylite::_macheps = 1e-5;
    bool fuzzylite::_debug = FL_DEBUG;
    bool fuzzylite::_logging = true;

    std::string fuzzylite::name() {
        return "fuzzylite";
    }

    std::string fuzzylite::fullname(){
        return name() + "-" + longVersion();
    }

    std::string fuzzylite::version() {
        return FL_VERSION;
    }

    std::string fuzzylite::longVersion() {
        return FL_VERSION "b" FL_DATE;
    }

    std::string fuzzylite::author() {
        return "Juan Rada-Vilela";
    }

    std::string fuzzylite::date() {
        return FL_DATE;
    }

    std::string fuzzylite::platform() {
#ifdef FL_UNIX
        return "Unix";
#elif defined FL_WINDOWS
        return "Windows";
#else 
        return "?";
#endif
    }

    std::string fuzzylite::configuration() {
        if (FL_DEBUG)
            return "Debug";
        return "Release";
    }

    std::string fuzzylite::floatingPoint() {
        scalar someScalar = 0;
        (void) someScalar;
        std::string type;

        std::ostringstream ss;
#ifdef FL_USE_FLOAT
        type = "float";
#else
        type = "double";
#endif
        ss << "fl::scalar is defined as \'" << type << "\' using " <<
                sizeof (someScalar) << " bytes";
        return ss.str();
    }

    void fuzzylite::setDebug(bool debug) {
        _debug = debug;
    }

    bool fuzzylite::debug() {
        return _debug;
    }

    void fuzzylite::setDecimals(int decimals) {
        _decimals = decimals;
    }

    int fuzzylite::decimals() {
        return _decimals;
    }

    void fuzzylite::setMachEps(scalar macheps) {
        _macheps = macheps;
    }

    scalar fuzzylite::macheps() {
        return _macheps;
    }

    void fuzzylite::setLogging(bool logging) {
        _logging = logging;
    }

    bool fuzzylite::logging() {
        return _logging;
    }

}
