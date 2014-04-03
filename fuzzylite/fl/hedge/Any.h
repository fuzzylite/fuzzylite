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
 * Any.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_ANY_H
#define FL_ANY_H

#include "fl/hedge/Hedge.h"

namespace fl {
    //Only this hedge has virtual methods due to its special case use.
    class FL_EXPORT Any : public Hedge {
    public:
        Any();
        virtual ~Any();
        
        virtual std::string name() const;
        virtual scalar hedge(scalar x) const;
        virtual Any* clone() const;
        
        static Hedge* constructor();
    };

}
#endif /* FL_ANY_H */
