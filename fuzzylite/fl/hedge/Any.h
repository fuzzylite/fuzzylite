/*
 * Any.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_ANY_H_
#define FL_ANY_H_

#include "fl/hedge/Hedge.h"

namespace fl {

    class Any : public Hedge {
    public:
        std::string name() const;
        scalar hedge(scalar x) const;
    };

} 
#endif /* FL_ANY_H_ */
