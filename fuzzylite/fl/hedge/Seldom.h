/*
 * Seldom.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_SELDOM_H
#define FL_SELDOM_H

#include "fl/hedge/Hedge.h"

namespace fl {

    class Seldom : public Hedge {
    public:
        std::string name() const;
        scalar hedge(scalar x) const;
    };

} 
#endif /* FL_SELDOM_H */
