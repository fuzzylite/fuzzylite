/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FL_TNORMFACTORY_H
#define FL_TNORMFACTORY_H

#include "fl/factory/ConstructionFactory.h"

#include "fl/norm/TNorm.h"

namespace fl {

    class FL_EXPORT TNormFactory : public ConstructionFactory<TNorm*> {
    public:
        TNormFactory();
        virtual ~TNormFactory() FL_OVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(TNormFactory)
    };
}
#endif  /* FL_TNORMFACTORY_H */

