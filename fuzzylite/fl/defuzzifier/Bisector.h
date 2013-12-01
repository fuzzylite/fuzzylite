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
 * File:   Bisector.h
 * Author: jcrada
 *
 * Created on 25 April 2013, 3:55 PM
 */

#ifndef FL_BISECTOR_H
#define	FL_BISECTOR_H

#include "fl/defuzzifier/IntegralDefuzzifier.h"

namespace fl {

    class FL_EXPORT Bisector : public IntegralDefuzzifier {
    public:
        Bisector(int resolution = defaultResolution());

        std::string className() const;
        scalar defuzzify(const Term* term, scalar minimum, scalar maximum) const;
        
        static Defuzzifier* constructor();
    };

}

#endif	/* FL_BISECTOR_H */

