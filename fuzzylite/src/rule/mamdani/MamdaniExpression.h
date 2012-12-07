/*
 * MamdaniExpression.h
 *
 *  Created on: 7/12/2012
 *      Author: jcrada
 */

#ifndef FL_MAMDANIEXPRESSION_H_
#define FL_MAMDANIEXPRESSION_H_

#include "../../scalar.h"

#include "../Rule.h"

#include "../../engine/Operator.h"
#include "../../variable/InputVariable.h"
#include "../../term/Term.h"
#include "../../hedge/Hedge.h"


#include <string>
#include <sstream>
#include <vector>



namespace fl {

    class MamdaniExpression {

    public:
        const bool isOperator;
        MamdaniExpression(bool isOperator)
                : isOperator(isOperator) {
        }
        virtual ~MamdaniExpression() {
        }

        virtual std::string toString() const = 0;
    };

    class MamdaniProposition: public MamdaniExpression {
    public:
        InputVariable* inputVariable;
        std::vector<Hedge*> hedges;
        Term* term;
        scalar weight;
        MamdaniProposition()
                : MamdaniExpression(false), inputVariable(NULL), term(NULL), weight(1.0) {
        }

        ~MamdaniProposition() {
        }

        std::string toString() const {
            std::stringstream ss;
            ss << inputVariable->getName() << " " << Rule::FL_IS << " ";
            for (std::size_t i = 0; i < hedges.size(); ++i) {
                ss << hedges[i]->name();
                if (i < hedges.size() - 1)
                    ss << " ";
            }
            ss << term->getName();
            if (not Op::IsEq(weight, 1.0))
                ss << " " << Rule::FL_WITH << " " << weight;
            return ss.str();
        }
    };

    class MamdaniOperator: public MamdaniExpression {
    public:
        std::string name;
        MamdaniExpression* left;
        MamdaniExpression* right;

        MamdaniOperator()
                : MamdaniExpression(true), left(NULL), right(NULL) {
        }
        ~MamdaniOperator() {
            if (left)
                delete left;
            if (right)
                delete right;
        }
        std::string toString() const {
            return name;
        }
    };

} /* namespace fl */
#endif /* FL_MAMDANIEXPRESSION_H_ */
