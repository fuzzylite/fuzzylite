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
                scalar bottomLeft = fl::nan,
                scalar topLeft = fl::nan,
                scalar topRight = fl::nan,
                scalar bottomRight = fl::nan);

        virtual ~PiShape();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;

        virtual void setBottomLeft(scalar a);
        virtual scalar getBottomLeft() const;

        virtual void setTopLeft(scalar b);
        virtual scalar getTopLeft() const;

        virtual void setTopRight(scalar d);
        virtual scalar getTopRight() const;

        virtual void setBottomRight(scalar c);
        virtual scalar getBottomRight() const;

        virtual PiShape* copy() const;
        static Term* constructor();
    };
}

#endif	/* FL_PISHAPE_H */

