/*
 Copyright (C) 2010-2016 by FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite(R).

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite(R) is a registered trademark of FuzzyLite Limited.
 */

#include "fl/fuzzylite.h"

namespace fl {


    int fuzzylite::_decimals = 3;
    std::ios_base::fmtflags fuzzylite::_scalarFormat = std::ios_base::fixed;
    scalar fuzzylite::_macheps = 1e-6;
    bool fuzzylite::_debugging = false;
    bool fuzzylite::_logging = true;

    std::string platform() {
#ifdef FL_UNIX
        return "Unix";
#elif defined FL_WINDOWS
        return "Windows";
#else
        return "?";
#endif
    }

    std::string floatingPoint() {
        scalar someScalar = 0;
        FL_IUNUSED(someScalar);
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
}
