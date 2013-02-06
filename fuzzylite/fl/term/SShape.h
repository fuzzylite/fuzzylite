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
 * File:   SShape.h
 * Author: jcrada
 *
 * Created on 22 December 2012, 4:43 PM
 */

#ifndef FL_SSHAPE_H
#define	FL_SSHAPE_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT SShape  : public Term{
    protected:
        scalar _start, _end;

    public:
        SShape(const std::string& name = "",
                scalar _start = -std::numeric_limits<scalar>::infinity(),
                scalar _end = std::numeric_limits<scalar>::infinity());


        std::string className() const;
        SShape* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setStart(scalar start);
        scalar getStart() const;

        void setEnd(scalar end);
        scalar getEnd() const;
    };
}

#endif	/* FL_SSHAPE_H */

