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
 * File:   DefuzzifierFactory.cpp
 * Author: jcrada
 * 
 * Created on 8 January 2013, 11:17 PM
 */

#include "fl/factory/DefuzzifierFactory.h"

#include "fl/defuzzifier/Centroid.h"
#include "fl/defuzzifier/Bisector.h"
#include "fl/defuzzifier/SmallestOfMaximum.h"
#include "fl/defuzzifier/LargestOfMaximum.h"
#include "fl/defuzzifier/MeanOfMaximum.h"
#include "fl/defuzzifier/WeightedAverage.h"
#include "fl/defuzzifier/WeightedSum.h"
#include "fl/Exception.h"

namespace fl {

    DefuzzifierFactory::DefuzzifierFactory() {
        registerClass(Bisector().className(), &(Bisector::constructor));
        registerClass(Centroid().className(), &(Centroid::constructor));
        registerClass(LargestOfMaximum().className(), &(LargestOfMaximum::constructor));
        registerClass(MeanOfMaximum().className(), &(MeanOfMaximum::constructor));
        registerClass(SmallestOfMaximum().className(), &(SmallestOfMaximum::constructor));
        registerClass(WeightedAverage().className(), &(WeightedAverage::constructor));
        registerClass(WeightedSum().className(), &(WeightedSum::constructor));
    }

    DefuzzifierFactory::~DefuzzifierFactory() {
    }

}
