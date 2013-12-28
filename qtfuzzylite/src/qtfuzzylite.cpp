/**
    This file is part of qtfuzzylite.

    qtfuzzylite is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    qtfuzzylite is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with qtfuzzylite.  If not, see <http://www.gnu.org/licenses/>.

    Juan Rada-Vilela, 01 February 2013
    jcrada@fuzzylite.com
 **/

/*
 * qtfuzzylite.h
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#include "fl/qt/qtfuzzylite.h"

namespace fl {
    namespace qt {

        std::string qtfuzzylite::name() {
            return "qtfuzzylite";
        }

        std::string qtfuzzylite::fullname() {
            return name() + "-" + longVersion();
        }

        std::string qtfuzzylite::version() {
            return FLQT_VERSION;
        }

        std::string qtfuzzylite::longVersion() {
            return FLQT_VERSION "b" FLQT_DATE;
        }

        std::string qtfuzzylite::author() {
            return "Juan Rada-Vilela";
        }

        std::string qtfuzzylite::date() {
            return FLQT_DATE;
        }

        std::string qtfuzzylite::platform() {
#ifdef FL_UNIX
            return "Unix";
#elif defined FL_WINDOWS
            return "Windows";
#else 
            return "?";
#endif
        }

        std::string qtfuzzylite::configuration() {
#ifdef FL_DEBUG
            return "Debug";
#else
            return "Release";
#endif
        }

        std::string qtfuzzylite::floatingPoint() {
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

    }
}

