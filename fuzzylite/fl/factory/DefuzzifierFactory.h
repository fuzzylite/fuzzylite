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
 * File:   DefuzzifierFactory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:17 PM
 */

#ifndef FL_DEFUZZIFIERFACTORY_H
#define	FL_DEFUZZIFIERFACTORY_H

#include "fl/fuzzylite.h"

#include "fl/factory/ConstructionFactory.h"

#include <string>
#include <vector>

namespace fl {
    class Defuzzifier;

    class FL_EXPORT DefuzzifierFactory : public ConstructionFactory<Defuzzifier*> {
    public:
        DefuzzifierFactory();
        virtual ~DefuzzifierFactory();

        virtual Defuzzifier* constructDefuzzifier(const std::string& key, int resolution) const;
    };
}
#endif	/* DEFUZZIFIERFACTORY_H */

