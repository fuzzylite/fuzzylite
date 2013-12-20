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
 * Term.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_TERM_H
#define FL_TERM_H

#include "fl/fuzzylite.h"

#include "fl/Operation.h"

#include <string>
#include <limits>
#include <iomanip>
#include <cmath>
#include <vector>

namespace fl {

    class FL_EXPORT Term {
    protected:
        std::string _name;

    public:

        Term(const std::string& name = "");
        virtual ~Term();

        /**
         * Sets the name of this term. It <b>must</b> be a valid id (see Op::makeValidId)
         * @param name
         */
        virtual void setName(const std::string& name);
        virtual std::string getName() const;
        
        virtual std::string toString() const;
        
        virtual std::string className() const = 0;
        virtual std::string parameters() const = 0;
        virtual void configure(const std::string& parameters) = 0;

        virtual scalar membership(scalar x) const = 0;

        virtual Term* copy() const = 0;
        
    };

}
#endif /* FL_TERM_H */
