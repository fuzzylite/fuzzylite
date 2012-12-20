/*
 * Extremely.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXTREMELY_H_
#define FL_EXTREMELY_H_

#include "fl/hedge/Hedge.h"

namespace fl {

    class Extremely : public Hedge {
    public:
        std::string name() const;
        scalar hedge(scalar x) const;
    };

} /* namespace fl */
#endif /* FL_EXTREMELY_H_ */
