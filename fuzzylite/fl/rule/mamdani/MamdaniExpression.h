/*
 * MamdaniExpression.h
 *
 *  Created on: 7/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIEXPRESSION_H
#define FL_MAMDANIEXPRESSION_H

#include "fl/definitions.h"

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
