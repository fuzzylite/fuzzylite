/*
 Copyright © 2010-2016 by FuzzyLite Limited.
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

    namespace fuzzylite {

        int decimals = 3;
        std::ios_base::fmtflags scalarFomat = std::ios_base::fixed;
        scalar macheps = 1e-6;
        bool debugging = false;
        bool logging = true;

        std::string name() {
            return "fuzzylite";
        }

        std::string library() {
            return name() + " " + version();
        }

        std::string version() {
            return "6.0";
        }

        std::string license() {
            return "FuzzyLite License";
        }

        std::string author() {
            return "Juan Rada-Vilela, Ph.D.";
        }

        std::string company() {
            return "FuzzyLite Limited";
        }

        std::string website() {
            return "http://www.fuzzylite.com/";
        }

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

        void setDebugging(bool debugging_) {
            debugging = debugging_;
        }

        bool isDebugging() {
            return debugging;
        }

        void setDecimals(int decimals_) {
            decimals = decimals_;
        }

        int getDecimals() {
            return decimals;
        }

        void setScalarFormat(std::ios_base::fmtflags scalarFormat_) {
            scalarFomat = scalarFormat_;
        }

        std::ios_base::fmtflags getScalarFormat() {
            return scalarFomat;
        }

        void setMachEps(scalar macheps_) {
            macheps = macheps_;
        }

        scalar getMachEps() {
            return macheps;
        }

        void setLogging(bool logging_) {
            logging = logging_;
        }

        bool isLogging() {
            return logging;
        }
    }
}
