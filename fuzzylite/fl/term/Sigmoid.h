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
 * Sigmoid.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_SIGMOID_H
#define FL_SIGMOID_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Sigmoid : public Term {
    protected:
        scalar _inflection;
        scalar _slope;
    public:
        Sigmoid(const std::string& name = "",
                scalar inflection = fl::nan,
                scalar slope = fl::nan);
        virtual ~Sigmoid();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);
        
        virtual scalar membership(scalar x) const;

        virtual void setInflection(scalar inflection);
        virtual scalar getInflection() const;

        virtual void setSlope(scalar slope);
        virtual scalar getSlope() const;
        
        virtual Sigmoid* copy() const;
        
        static Term* constructor();
    };

}
#endif /* FL_SIGMOID_H */
