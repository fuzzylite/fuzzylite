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

#include "fuzzylite/fuzzylite.h"

namespace fuzzylite {

    int fuzzylite::_decimals = 3;
    std::ios_base::fmtflags fuzzylite::_scalarFormat = std::ios_base::fixed;
    // TODO: replace macheps with atol
    scalar fuzzylite::_macheps = 1e-6;
    scalar fuzzylite::_atol = 1e-3;
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
        ss << "fl::scalar is defined as \'" << type << "\' using " << sizeof(someScalar) << " bytes";
        return ss.str();
    }
}
