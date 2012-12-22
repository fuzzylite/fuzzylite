/*
 * MamdaniRule.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIRULE_H_
#define FL_MAMDANIRULE_H_

#include "fl/rule/Rule.h"

#include <string>

namespace fl {
    class Engine;

    class MamdaniRule : public Rule {
    public:
        MamdaniRule();
        ~MamdaniRule();

        static MamdaniRule* parse(const std::string& rule, const Engine* engine);
        static void main();
    };

} 
#endif /* FL_MAMDANIRULE_H_ */
