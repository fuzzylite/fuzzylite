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

#ifndef FL_WEIGHTEDDEFUZZIFIER_H
#define FL_WEIGHTEDDEFUZZIFIER_H

#include "fl/defuzzifier/Defuzzifier.h"

namespace fl {
    class Activated;

    class FL_API WeightedDefuzzifier : public Defuzzifier {
    public:

        enum Type {
            Automatic, TakagiSugeno, Tsukamoto
        };
        static std::string typeName(Type);

        explicit WeightedDefuzzifier(Type type = Automatic);
        explicit WeightedDefuzzifier(const std::string& type);
        virtual ~WeightedDefuzzifier() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(WeightedDefuzzifier)

        virtual void setType(Type type);
        virtual Type getType() const;
        virtual std::string getTypeName() const;
        virtual Type inferType(const Term* term) const;
        virtual bool isMonotonic(const Term* term) const;

        virtual scalar tsukamoto(const Term* monotonic, scalar activationDegree,
                scalar minimum, scalar maximum) const;

    protected:
        Type _type;

    };

}

#endif  /* FL_WEIGHTEDDEFUZZIFIER_H */

