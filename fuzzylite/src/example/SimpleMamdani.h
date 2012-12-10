/*
 * SimpleMamdani.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_SIMPLEMAMDANI_H_
#define FL_SIMPLEMAMDANI_H_

#include "../Headers.h"

#include "Example.h"

namespace fl {

    class SimpleMamdani: public Example {
    protected:
        Engine* _engine;

    public:
        SimpleMamdani();
        ~SimpleMamdani();

        std::string name() const;
        Engine* engine() const;

        void create();
        void test();

        static void main();
    };

} /* namespace fl */

#endif /* FL_SIMPLEMAMDANI_H_ */
