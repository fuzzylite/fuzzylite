/*
 * Example.h
 *
 *  Created on: 7/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXAMPLE_H_
#define FL_EXAMPLE_H_

#include <string>

#include "fl/engine/Engine.h"

namespace fl {

    class Example {
    public:
        Engine* engine;

        Example() :
        engine(NULL) { }

        virtual ~Example() {
            if (engine) delete engine;
        }

        virtual std::string name() const {
            return engine ? engine->getName() : "NULL";
        }

        virtual void test() = 0;

    };
}

#endif /* FL_EXAMPLE_H_ */
