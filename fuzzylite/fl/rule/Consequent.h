/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * Consequent.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CONSEQUENT_H
#define FL_CONSEQUENT_H

#include "fl/fuzzylite.h"

#include <string>
#include <vector>

namespace fl {
    class Engine;
    class Proposition;
    class TNorm;

    class FL_EXPORT Consequent {
    protected:
        std::vector<Proposition*> _conclusions;

    public:
        Consequent();
        virtual ~Consequent();

        virtual std::vector<Proposition*> conclusions() const;

        virtual void load(const std::string& consequent, const Engine* engine);

        virtual void modify(scalar strength, const TNorm* activation);

        virtual std::string toString() const;
    };

}

#endif /* FL_CONSEQUENT_H */
