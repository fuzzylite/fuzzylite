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
 * File:   HedgeFactory.cpp
 * Author: jcrada
 * 
 * Created on 11 January 2013, 8:51 PM
 */

#include "fl/factory/HedgeFactory.h"

#include "fl/hedge/Any.h"
#include "fl/hedge/Extremely.h"
#include "fl/hedge/Not.h"
#include "fl/hedge/Seldom.h"
#include "fl/hedge/Somewhat.h"
#include "fl/hedge/Very.h"

#include "fl/Exception.h"

namespace fl {

    HedgeFactory::HedgeFactory() { }

    HedgeFactory::~HedgeFactory() { }

    Hedge* HedgeFactory::create(const std::string& name) const {
        if (name == "") return NULL;
        if (name == Any().name()) return new Any;
        if (name == Extremely().name()) return new Extremely;
        if (name == Not().name()) return new Not;
        if (name == Seldom().name()) return new Seldom;
        if (name == Somewhat().name()) return new Somewhat;
        if (name == Very().name()) return new Very;
        throw fl::Exception("[factory error] hedge of name <" + name + " not recognized", FL_AT);
    }

    std::vector<std::string> HedgeFactory::available() const {
        std::vector<std::string> result;
        result.push_back(Any().name());
        result.push_back(Extremely().name());
        result.push_back(Not().name());
        result.push_back(Seldom().name());
        result.push_back(Somewhat().name());
        result.push_back(Very().name());
        return result;
    }


}
