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
 * File:   HedgeFactory.h
 * Author: jcrada
 *
 * Created on 11 January 2013, 8:51 PM
 */

#ifndef FL_HEDGEFACTORY_H
#define	FL_HEDGEFACTORY_H

#include "fl/fuzzylite.h"

#include "fl/factory/Factory.h"
#include "fl/hedge/Hedge.h"

namespace fl {
    
    class FL_EXPORT HedgeFactory : public Factory<Hedge*>{
    public:
        HedgeFactory();
        virtual ~HedgeFactory();
        
    };
}
#endif	/* FL_HEDGEFACTORY_H */

