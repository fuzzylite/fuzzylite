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
                scalar center = fl::nan,
                scalar width = fl::nan,
                scalar slope = fl::nan);
        virtual ~Bell();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);
        
        virtual scalar membership(scalar x) const;

        virtual void setCenter(scalar center);
        virtual scalar getCenter() const;

        virtual void setWidth(scalar width);
        virtual scalar getWidth() const;

        virtual void setSlope(scalar slope);
        virtual scalar getSlope() const;

        virtual Bell* copy() const;
        
        static Term* constructor();
        
    };

}
#endif /* FL_BELL_H */
