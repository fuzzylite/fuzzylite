/*
 * qtfuzzylite.h
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#include <fl/fuzzylite.h>

#ifndef FLQT_DEFINITIONS_H_
#define FLQT_DEFINITIONS_H_

#ifndef FLQT_VERSION
#define FLQT_VERSION "?"
#endif

#ifndef FLQT_DATE
#define FLQT_DATE "?"
#endif

namespace fl {
    namespace qt {

        class qtfuzzylite {
        public:
            static std::string name();
            static std::string fullname();
            static std::string shortVersion();
            static std::string longVersion();
            static std::string author();

            static std::string date();
            static std::string platform();
            static std::string configuration();
            static std::string floatingPoint();

            static bool logEnabled();
        };
    }
}

#endif /* FLQT_DEFINITIONS_H_ */
