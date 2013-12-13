/* 
 * File:   Tsukamoto.h
 * Author: jcrada
 *
 * Created on 13 December 2013, 4:24 PM
 */

#ifndef FL_TSUKAMOTO_H
#define	FL_TSUKAMOTO_H

#include "fl/fuzzylite.h"

namespace fl {
    class Thresholded;
    
    class FL_EXPORT Tsukamoto {
    public:
        static scalar tsukamoto(const Thresholded* term, scalar minimum, scalar maximum);
    };

}

#endif	/* FL_TSUKAMOTO_H */

