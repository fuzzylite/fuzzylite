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
 * File:   TNormFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013) return  11:20 PM
 */

#include "fl/factory/TNormFactory.h"

#include "fl/norm/t/AlgebraicProduct.h"
#include "fl/norm/t/BoundedDifference.h"
#include "fl/norm/t/DrasticProduct.h"
#include "fl/norm/t/EinsteinProduct.h"
#include "fl/norm/t/HamacherProduct.h"
#include "fl/norm/t/Minimum.h"

namespace fl {

    TNormFactory::TNormFactory() {
        registerClass(Minimum().className(), &(Minimum::constructor));
        registerClass(AlgebraicProduct().className(), &(AlgebraicProduct::constructor));
        registerClass(BoundedDifference().className(), &(BoundedDifference::constructor));
        registerClass(DrasticProduct().className(), &(DrasticProduct::constructor));
        registerClass(EinsteinProduct().className(), &(EinsteinProduct::constructor));
        registerClass(HamacherProduct().className(), &(HamacherProduct::constructor));
    }

    TNormFactory::~TNormFactory() {
    }

}
