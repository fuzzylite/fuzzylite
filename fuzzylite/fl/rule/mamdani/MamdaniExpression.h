/*
 * MamdaniExpression.h
 *
 *  Created on: 7/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIEXPRESSION_H
#define FL_MAMDANIEXPRESSION_H

#include "fl/scalar.h"

#include "fl/rule/Rule.h"

#include "fl/operator/Operator.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/term/Term.h"
#include "fl/hedge/Hedge.h"


#include <string>
#include <sstream>
#include <vector>



namespace fl {

    class MamdaniExpression {
    public:
        const bool isOperator;

        MamdaniExpression(bool isOperator);
        virtual ~MamdaniExpression();

        virtual std::string toString() const = 0;
    };

    class MamdaniProposition : public MamdaniExpression {
    public:
        Variable* variable;
        std::vector<Hedge*> hedges;
        Term* term;

        MamdaniProposition();

        std::string toString() const;
    };


    class MamdaniOperator : public MamdaniExpression {
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
