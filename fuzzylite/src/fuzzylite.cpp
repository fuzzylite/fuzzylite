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

#include "fl/fuzzylite.h"

namespace fl {

    int fuzzylite::_decimals = 3;
    scalar fuzzylite::_macheps = 1e-6;
    bool fuzzylite::_debug = false;
    bool fuzzylite::_logging = true;

    std::string fuzzylite::name() {
        return "fuzzylite";
    }

    std::string fuzzylite::fullname() {
        return name() + "-" + longVersion();
    }

    std::string fuzzylite::version() {
        return FL_VERSION;
    }

    std::string fuzzylite::longVersion() {
        return FL_VERSION "b" FL_DATE;
    }

    std::string fuzzylite::license() {
        return "GNU Lesser General Public License v3.0";
    }

    std::string fuzzylite::author() {
        return "Juan Rada-Vilela, Ph.D.";
    }

    std::string fuzzylite::company() {
        return "FuzzyLite Limited";
    }

    std::string fuzzylite::website() {
        return "http://www.fuzzylite.com/";
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
