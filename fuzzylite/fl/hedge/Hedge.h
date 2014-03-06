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
 * Hedge.h
 *
 *  Created on: 3/12/2012
 *      Author: jcrada
 */

#ifndef FL_HEDGE_H
#define FL_HEDGE_H

#include "fl/fuzzylite.h"



#include <string>

namespace fl {

    class FL_EXPORT Hedge {
    public:

        Hedge() { }

        virtual ~Hedge() { }

        virtual std::string name() const = 0;
        virtual scalar hedge(scalar x) const = 0;

    };
}

#endif /* FL_HEDGE_H */
