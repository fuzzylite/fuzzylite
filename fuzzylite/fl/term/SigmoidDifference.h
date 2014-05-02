// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

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
                scalar left = fl::nan,
                scalar rising = fl::nan,
                scalar falling = fl::nan,
                scalar right = fl::nan);
        virtual ~SigmoidDifference();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual scalar membership(scalar x) const;

        virtual void setLeft(scalar leftInflection);
        virtual scalar getLeft() const;

        virtual void setRising(scalar risingSlope);
        virtual scalar getRising() const;

        virtual void setFalling(scalar fallingSlope);
        virtual scalar getFalling() const;

        virtual void setRight(scalar rightInflection);
        virtual scalar getRight() const;

        virtual SigmoidDifference* clone() const;

        static Term* constructor();
    };
}
#endif	/* FL_SIGMOIDDIFFERENCE_H */

