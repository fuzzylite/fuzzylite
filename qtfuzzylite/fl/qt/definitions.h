/*
 * definitions.h
 *
 *  Created on: 18/12/2012
 *      Author: jcrada
 */

#include <fl/definitions.h>
 
#ifndef FLQT_DEFINITIONS_H_
#define FLQT_DEFINITIONS_H_

#ifndef FLQT_VERSION
#define FLQT_VERSION "?"
#endif

#ifndef FLQT_DATE
#define FLQT_DATE "?"
#endif

namespace fl{
    namespace qt{
        static std::string version(){
            return FLQT_VERSION;
        }
        static std::string date(){
            return FLQT_DATE;
        }
        static std::string longVersion(){
            return FLQT_VERSION "-" FLQT_DATE;
        }
    }
}

#endif /* FLQT_DEFINITIONS_H_ */
