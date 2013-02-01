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

        std::string qtfuzzylite::shortVersion() {
            return FLQT_VERSION;
        }

        std::string qtfuzzylite::longVersion() {
            return FLQT_VERSION "b" FLQT_DATE;
        }

        std::string qtfuzzylite::author() {
            return "Juan Rada-Vilela, jcrada@fuzzylite.com";
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

        bool qtfuzzylite::logEnabled() {
#ifdef FL_NO_LOG
            return false;
#else
            return true;
#endif
        }
    }
}

