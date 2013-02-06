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
 * File:   PiShape.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 5:15 PM
 */

#ifndef FL_PISHAPE_H
#define	FL_PISHAPE_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT PiShape : public Term {
    protected:
        scalar _bottomLeft;
        scalar _topLeft;
        scalar _topRight;
        scalar _bottomRight;

    public:
        PiShape(const std::string& name = "",
                scalar bottomLeft = std::numeric_limits<scalar>::quiet_NaN(),
                scalar topLeft = std::numeric_limits<scalar>::quiet_NaN(),
                scalar topRight = std::numeric_limits<scalar>::quiet_NaN(),
                scalar bottomRight = std::numeric_limits<scalar>::quiet_NaN());


        std::string className() const;
        PiShape* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;


        void setBottomLeft(scalar a);
        scalar getBottomLeft() const;

        void setTopLeft(scalar b);
        scalar getTopLeft() const;

        void setTopRight(scalar d);
        scalar getTopRight() const;

        void setBottomRight(scalar c);
        scalar getBottomRight() const;

    };
}

#endif	/* FL_PISHAPE_H */

