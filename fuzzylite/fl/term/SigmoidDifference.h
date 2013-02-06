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
 * File:   SigmoidDifference.h
 * Author: jcrada
 *
 * Created on 30 December 2012, 5:58 PM
 */

#ifndef FL_SIGMOIDDIFFERENCE_H
#define	FL_SIGMOIDDIFFERENCE_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT SigmoidDifference : public Term {
    protected:
        scalar _left;
        scalar _rising;
        scalar _falling;
        scalar _right;
        
    public:
        SigmoidDifference(const std::string& name = "",
                scalar left = std::numeric_limits<scalar>::quiet_NaN(),
                scalar rising = std::numeric_limits<scalar>::quiet_NaN(),
                scalar falling = std::numeric_limits<scalar>::quiet_NaN(),
                scalar right = std::numeric_limits<scalar>::quiet_NaN());

        std::string className() const;
        SigmoidDifference* copy() const;

        scalar membership(scalar x) const;

        std::string toString() const;


        void setLeft(scalar leftInflection);
        scalar getLeft() const;

        void setRising(scalar risingSlope);
        scalar getRising() const;
        
        void setFalling(scalar fallingSlope);
        scalar getFalling() const;
        
        void setRight(scalar rightInflection);
        scalar getRight() const;



    };
}
#endif	/* FL_SIGMOIDDIFFERENCE_H */

