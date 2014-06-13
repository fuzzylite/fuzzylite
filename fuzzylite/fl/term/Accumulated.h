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
 * Accumulated.h
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#ifndef FL_ACCUMULATED_H
#define FL_ACCUMULATED_H

#include "fl/term/Term.h"

#include <memory>
#include <vector>

namespace fl {

    class Activated;
    class SNorm;
    class TNorm;
    
    class FL_EXPORT Accumulated : public Term {
    private:
        void copyFrom(const Accumulated& source);
    protected:
        std::vector<Activated*> _terms;
        scalar _minimum, _maximum;
        std::auto_ptr<SNorm> _accumulation;
    public:
        Accumulated(const std::string& name = "",
                scalar minimum = -fl::inf,
                scalar maximum = fl::inf,
                SNorm* accumulation = NULL);
        Accumulated(const Accumulated& source);
        Accumulated& operator=(const Accumulated& rhs);

        virtual ~Accumulated();

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual Accumulated* clone() const;

        virtual scalar membership(scalar x) const;

        virtual std::string toString() const;


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
        virtual void addTerm(const Term* term, scalar degree, const TNorm* activation);
        virtual void addTerm(Activated* term);
        virtual Activated* getTerm(int index) const;
        virtual Activated* removeTerm(int index);
        virtual int numberOfTerms() const;
        virtual const std::vector<Activated*>& terms() const;
        virtual std::vector<Activated*>& terms();
        virtual bool isEmpty() const;
        virtual void clear();
    };

}
#endif /* FL_ACCUMULATED_H */
