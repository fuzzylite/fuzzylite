/*
 * Not.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_NOT_H_
#define FL_NOT_H_

#include "Hedge.h"

namespace fl {

    class Not: public Hedge {
    public:
        std::string name() const;
        scalar hedge(scalar x) const;
    };

} /* namespace fl */
#endif /* FL_NOT_H_ */
