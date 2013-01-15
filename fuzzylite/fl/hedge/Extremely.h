/*
 * Extremely.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXTREMELY_H
#define FL_EXTREMELY_H

#include "fl/hedge/Hedge.h"

namespace fl {

    class Extremely : public Hedge {
    public:
        std::string name() const;
        scalar hedge(scalar x) const;
    };

} 
#endif /* FL_EXTREMELY_H */
