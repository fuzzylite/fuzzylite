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

#ifndef FL_ACCUMULATED_H
#define FL_ACCUMULATED_H

#include "fl/term/Term.h"

#include <vector>

namespace fl {

    class Activated;
    class SNorm;
    class TNorm;

    class FL_API Accumulated : public Term {
    private:
        std::vector<Activated*> _terms;
        scalar _minimum, _maximum;
        FL_unique_ptr<SNorm> _accumulation;
        
        void copyFrom(const Accumulated& source);
    public:
        explicit Accumulated(const std::string& name = "",
                scalar minimum = fl::nan,
                scalar maximum = fl::nan,
                SNorm* accumulation = fl::null);
        Accumulated(const Accumulated& other);
        Accumulated& operator=(const Accumulated& other);
        virtual ~Accumulated() FL_IOVERRIDE;
        FL_DEFAULT_MOVE(Accumulated)

        virtual std::string className() const FL_IOVERRIDE;
        virtual std::string parameters() const FL_IOVERRIDE;
        virtual void configure(const std::string& parameters) FL_IOVERRIDE;

        virtual Accumulated* clone() const FL_IOVERRIDE;

        virtual scalar membership(scalar x) const FL_IOVERRIDE;
        virtual scalar activationDegree(const Term* forTerm) const;

        virtual std::string toString() const FL_IOVERRIDE;


        virtual void setMinimum(scalar minimum);
        virtual scalar getMinimum() const;

        virtual void setMaximum(scalar maximum);
        virtual scalar getMaximum() const;

        virtual void setRange(scalar minimum, scalar maximum);
        virtual scalar range() const;

        virtual void setAccumulation(SNorm* accumulation);
        virtual SNorm* getAccumulation() const;

        /**
         * Operations for std::vector _terms
         */
        virtual void addTerm(const Term* term, scalar degree, const TNorm* implication);
        virtual void addTerm(Activated* term);
        virtual Activated* getTerm(std::size_t index) const;
        virtual Activated* removeTerm(std::size_t index);
        virtual std::size_t numberOfTerms() const;
        virtual const std::vector<Activated*>& terms() const;
        virtual std::vector<Activated*>& terms();
        virtual bool isEmpty() const;
        virtual void clear();
    };

}
#endif /* FL_ACCUMULATED_H */
