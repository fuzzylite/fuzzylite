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


namespace fl {

    HedgeFactory::HedgeFactory() {
        registerClass(Any().name(), &(Any::constructor));
        registerClass(Extremely().name(), Extremely::constructor);
        registerClass(Not().name(), Not::constructor);
        registerClass(Seldom().name(), Seldom::constructor);
        registerClass(Somewhat().name(), Somewhat::constructor);
        registerClass(Very().name(), Very::constructor);
    }

    HedgeFactory::~HedgeFactory() {
    }

}
