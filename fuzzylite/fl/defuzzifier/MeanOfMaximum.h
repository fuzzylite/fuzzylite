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
 * File:   MeanOfMaximum.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 6:38 PM
 */

#ifndef FL_MEANOFMAXIMUM_H
#define	FL_MEANOFMAXIMUM_H

#include "fl/defuzzifier/IntegralDefuzzifier.h"

namespace fl {

    class FL_EXPORT MeanOfMaximum : public IntegralDefuzzifier {
    public:
        MeanOfMaximum(int resolution = defaultResolution());
        virtual ~MeanOfMaximum();

        virtual std::string className() const;
        virtual scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const;
        
        static Defuzzifier* constructor();
    };
}

#endif	/* FL_MEANOFMAXIMUM_H */

