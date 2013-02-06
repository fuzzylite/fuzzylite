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
 * Bell.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_BELL_H
#define FL_BELL_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Bell : public Term {
    protected:
        scalar _center;
        scalar _width;
        scalar _slope;
    public:
        Bell(const std::string& name = "",
                scalar center = std::numeric_limits<scalar>::quiet_NaN(),
                scalar width = std::numeric_limits<scalar>::quiet_NaN(),
                scalar slope = std::numeric_limits<scalar>::quiet_NaN());
        ~Bell();

        std::string className() const;
        Bell* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;

        void setCenter(scalar center);
        scalar getCenter() const;

        void setWidth(scalar width);
        scalar getWidth() const;

        void setSlope(scalar slope);
        scalar getSlope() const;

    };

}
#endif /* FL_BELL_H */
