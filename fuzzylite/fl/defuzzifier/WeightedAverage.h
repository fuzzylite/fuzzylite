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
 * File:   WeightedAverage.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 4:15 PM
 */

#ifndef FL_WEIGHTEDAVERAGE_H
#define	FL_WEIGHTEDAVERAGE_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {
    class Activated;

    class FL_EXPORT WeightedAverage : public Defuzzifier {
    public:

        WeightedAverage();
        virtual ~WeightedAverage();

        virtual std::string className() const;
        virtual scalar defuzzify(const Term* term,
                scalar minimum, scalar maximum) const;
        virtual WeightedAverage* clone() const;
        
        static Defuzzifier* constructor();
    };
}

#endif	/* FL_WEIGHTEDAVERAGE_H */

