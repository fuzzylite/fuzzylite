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


#ifndef FL_TERM_H
#define FL_TERM_H

#include "fl/fuzzylite.h"

#include "fl/Operation.h"

#include <cmath>
#include <string>
#include <vector>

namespace fl {
    class Engine;

    class FL_API Term {
    private:
        std::string _name;
        scalar _height;
    public:

        explicit Term(const std::string& name = "", scalar height = 1.0);
        virtual ~Term();
        FL_DEFAULT_COPY_AND_MOVE(Term)

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        virtual void setHeight(scalar height);
        virtual scalar getHeight() const;

        virtual std::string toString() const;

        virtual std::string className() const = 0;
        virtual std::string parameters() const = 0;
        virtual void configure(const std::string& parameters) = 0;

        virtual scalar membership(scalar x) const = 0;

        virtual Term* clone() const = 0;

        //FIXME: This should not be static, and may not be here either.
        static void updateReference(Term* term, const Engine* engine);
    };

}
#endif /* FL_TERM_H */
