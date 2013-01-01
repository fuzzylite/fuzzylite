/*
 * Example1.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXAMPLE1_H_
#define FL_EXAMPLE1_H_

#include "fl/example/Example.h"

namespace fl {

    class Example1 : public Example {
    protected:
        void create();
    public:
        Example1();

        void test();

        static void main();
    };

} 

#endif /* FL_EXAMPLE1_H_ */
