/*
 * Seldom.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_SELDOM_H_
#define FL_SELDOM_H_

#include "Hedge.h"

namespace fl {

    class Seldom: public Hedge {
    public:
        std::string name() const;
        scalar hedge(scalar x) const;
    };

} /* namespace fl */
#endif /* FL_SELDOM_H_ */
