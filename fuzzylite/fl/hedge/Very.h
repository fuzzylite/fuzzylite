/*
 * Very.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_VERY_H
#define FL_VERY_H

#include "fl/hedge/Hedge.h"

namespace fl {

    flclass Very : public Hedge {
    public:
        std::string name() const;
        scalar hedge(scalar x) const;
    };

} 
#endif /* FL_VERY_H */
