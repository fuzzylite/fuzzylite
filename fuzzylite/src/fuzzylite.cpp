/*
 * fuzzylite.cpp
 *
 *  Created on 1 February 2013, 10:47 AM
 *      Author: jcrada
 */

#include "fl/fuzzylite.h"

namespace fl {

    std::string fuzzylite::name() {
        return "fuzzylite";
    }

    std::string fuzzylite::fullname() {
        return name() + "-" + longVersion();
    }

    std::string fuzzylite::shortVersion() {
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

    int fuzzylite::decimals() {
        return FL_DECIMALS;
    }

    scalar fuzzylite::precision() {
        return FL_PRECISION;
    }

    int fuzzylite::defaultDivisions() {
        return FL_DIVISIONS;
    }

    bool fuzzylite::logEnabled() {
#ifdef FL_NO_LOG
        return false;
#else
        return true;
#endif
    }

}
