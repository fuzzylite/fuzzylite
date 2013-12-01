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
 * File:   TermFactory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:18 PM
 */

#ifndef FL_TERMFACTORY_H
#define	FL_TERMFACTORY_H

#include "fl/fuzzylite.h"

#include "fl/factory/Factory.h"
#include "fl/term/Term.h"

#include <vector>
#include <string>

namespace fl {

    class FL_EXPORT TermFactory : public Factory<Term*>{
    public:
        TermFactory();
        virtual ~TermFactory();

        virtual Term* createInstance(const std::string& className,
                const std::vector<scalar>& params) const;
    };
}
#endif	/* FL_TERMFACTORY_H */

