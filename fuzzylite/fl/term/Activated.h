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

#ifndef FL_ACTIVATED_H
#define FL_ACTIVATED_H

#include "fl/term/Term.h"

namespace fl {
    class TNorm;

    class FL_API Activated : public Term {
    protected:
        const Term* _term;
        scalar _degree;
        const TNorm* _implication;

    public:
        explicit Activated(const Term* term = fl::null, scalar degree = 1.0,
                const TNorm* implication = fl::null);
        virtual ~Activated() FL_IOVERRIDE;
        FL_DEFAULT_COPY_AND_MOVE(Activated)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;
        virtual std::string toString() const FL_IOVERRIDE;

        virtual void setTerm(const Term* term);
        virtual const Term* getTerm() const;

        virtual void setDegree(scalar degree);
        virtual scalar getDegree() const;

        virtual void setImplication(const TNorm* implication);
        virtual const TNorm* getImplication() const;

        virtual Activated* clone() const FL_IOVERRIDE;
    };

}
#endif /* FL_ACTIVATED_H */
