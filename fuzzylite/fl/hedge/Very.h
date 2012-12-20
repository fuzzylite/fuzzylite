/*
 * Very.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_VERY_H_
#define FL_VERY_H_

#include "fl/hedge/Hedge.h"

namespace fl {

    class Very : public Hedge {
    public:
        std::string name() const;
        scalar hedge(scalar x) const;
    };

} /* namespace fl */
#endif /* FL_VERY_H_ */
