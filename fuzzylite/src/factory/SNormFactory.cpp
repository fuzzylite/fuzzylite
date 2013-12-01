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
 * File:   SNormFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:20 PM
 */

#include "fl/factory/SNormFactory.h"

#include "fl/norm/s/AlgebraicSum.h"
#include "fl/norm/s/BoundedSum.h"
#include "fl/norm/s/DrasticSum.h"
#include "fl/norm/s/EinsteinSum.h"
#include "fl/norm/s/HamacherSum.h"
#include "fl/norm/s/Maximum.h"
#include "fl/norm/s/NormalizedSum.h"

namespace fl {

    SNormFactory::SNormFactory() {
        registerClass(AlgebraicSum().className(), &(AlgebraicSum::constructor));
        registerClass(BoundedSum().className(), &(BoundedSum::constructor));
        registerClass(DrasticSum().className(), &(DrasticSum::constructor));
        registerClass(EinsteinSum().className(), &(EinsteinSum::constructor));
        registerClass(HamacherSum().className(), &(HamacherSum::constructor));
        registerClass(Maximum().className(), &(Maximum::constructor));
        registerClass(NormalizedSum().className(), &(NormalizedSum::constructor));
    }

    SNormFactory::~SNormFactory() {
    }

}
