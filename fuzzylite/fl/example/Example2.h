/*
 * Example2.h
 *
 *  Created on: 17/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXAMPLE2_H_
#define FL_EXAMPLE2_H_

#include <fl/Headers.h>

#include "fl/example/Example.h"

namespace fl {

    class Example2 : public Example {
    protected:
        void create();

    public:
        Example2();

        void test();

        static void main();

    };

} 
#endif /* FL_EXAMPLE2_H_ */
