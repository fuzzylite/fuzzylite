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
 * MamdaniExpression.h
 *
 *  Created on: 7/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIEXPRESSION_H
#define FL_MAMDANIEXPRESSION_H

#include "fl/fuzzylite.h"

#include <string>
#include <vector>



namespace fl {
    class Variable;
    class Hedge;
    class Term;
    
    class FL_EXPORT MamdaniExpression {
    public:
        const bool isOperator;

        MamdaniExpression(bool isOperator);
        virtual ~MamdaniExpression();

        virtual std::string toString() const = 0;
    };

    class FL_EXPORT MamdaniProposition : public MamdaniExpression {
    public:
        Variable* variable;
        std::vector<Hedge*> hedges;
        Term* term;

        MamdaniProposition();

        std::string toString() const;
    };


    class FL_EXPORT MamdaniOperator : public MamdaniExpression {
    public:
        std::string name;
        MamdaniExpression* left;
        MamdaniExpression* right;

        MamdaniOperator();
        ~MamdaniOperator();

        std::string toString() const;
    };

}
#endif /* FL_MAMDANIEXPRESSION_H */
