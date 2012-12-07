/*
 * Example.h
 *
 *  Created on: 7/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXAMPLE_H_
#define FL_EXAMPLE_H_

#include <string>

namespace fl{
    class Engine;

    class Example{
    public:
        Example(){}
        virtual ~Example(){}

        virtual std::string name() const = 0;

        virtual void create() = 0;
        virtual void test() = 0;
        virtual Engine* engine() const = 0;


    };
}


#endif /* FL_EXAMPLE_H_ */
