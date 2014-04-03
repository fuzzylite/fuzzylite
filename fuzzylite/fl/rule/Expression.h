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
 * Expression.h
 *
 *  Created on: 7/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXPRESSION_H
#define FL_EXPRESSION_H

#include "fl/fuzzylite.h"

#include <string>
#include <vector>



namespace fl {
    class Variable;
    class Hedge;
    class Term;

    class FL_EXPORT Expression {
    public:

        Expression();
        virtual ~Expression();

        virtual Expression* clone() const = 0;
        virtual std::string toString() const = 0;

    };

    class FL_EXPORT Proposition : public Expression {
    public:
        Variable* variable;
        std::vector<Hedge*> hedges;
        Term* term;

        Proposition();
        virtual ~Proposition();
        
        virtual Proposition* clone() const;
        virtual std::string toString() const;


    };

    class FL_EXPORT Operator : public Expression {
    public:
        std::string name;
        Expression* left;
        Expression* right;

        Operator();
        Operator(const Operator& copy);
        Operator& operator=(const Operator& rhs);
        virtual ~Operator();

        virtual Operator* clone() const;
        virtual std::string toString() const;


    };

}
#endif /* FL_FUZZYEXPRESSION_H */
