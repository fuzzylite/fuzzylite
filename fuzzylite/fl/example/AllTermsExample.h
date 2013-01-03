/* 
 * File:   AllTermsExample.h
 * Author: jcrada
 *
 * Created on 3 January 2013, 6:38 PM
 */

#ifndef FL_ALLTERMSEXAMPLE_H
#define	FL_ALLTERMSEXAMPLE_H

#include "fl/example/Example.h"

namespace fl {

    class AllTermsExample : public Example {
    protected:
        void create();
    public:
        AllTermsExample();

        void test();

    };
} 


#endif	/* FL_ALLTERMSEXAMPLE_H */

