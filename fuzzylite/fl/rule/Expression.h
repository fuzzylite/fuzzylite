/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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
        bool isOperator;

        Expression(bool isOperator);
        virtual ~Expression();

        virtual std::string toString() const = 0;
    };

    class FL_EXPORT Proposition : public Expression {
    public:
        Variable* variable;
        std::vector<Hedge*> hedges;
        Term* term;

        Proposition();

        std::string toString() const;
    };


    class FL_EXPORT Operator : public Expression {
    public:
        std::string name;
        Expression* left;
        Expression* right;

        Operator();
        ~Operator();

        std::string toString() const;
    };

}
#endif /* FL_FUZZYEXPRESSION_H */
