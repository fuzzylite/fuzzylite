/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#ifndef FL_DEFUZZIFIERFACTORY_H
#define FL_DEFUZZIFIERFACTORY_H

#include "fl/factory/ConstructionFactory.h"

#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/defuzzifier/IntegralDefuzzifier.h"
#include "fl/defuzzifier/WeightedDefuzzifier.h"

namespace fl {

    class FL_API DefuzzifierFactory : public ConstructionFactory<Defuzzifier*> {
    public:
        DefuzzifierFactory();
        virtual ~DefuzzifierFactory() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(DefuzzifierFactory)

        virtual Defuzzifier* constructDefuzzifier(const std::string& key,
                int resolution, WeightedDefuzzifier::Type) const;

        virtual Defuzzifier* constructDefuzzifier(const std::string& key, int resolution) const;

        virtual Defuzzifier* constructDefuzzifier(const std::string& key, WeightedDefuzzifier::Type type);
    };
}
#endif  /* DEFUZZIFIERFACTORY_H */

