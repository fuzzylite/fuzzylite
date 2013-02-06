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
 * Hedge.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_HEDGE_H
#define FL_HEDGE_H

#include "fl/fuzzylite.h"



#include <string>

namespace fl {

    class FL_EXPORT Hedge {
    public:

        Hedge() { }

        virtual ~Hedge() { }

        virtual std::string name() const = 0;
        virtual scalar hedge(scalar x) const = 0;

    };
}

#endif /* FL_HEDGE_H */
