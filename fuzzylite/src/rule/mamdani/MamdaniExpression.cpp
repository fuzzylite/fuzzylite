#include "fl/rule/mamdani/MamdaniExpression.h"

namespace fl {

    MamdaniExpression::MamdaniExpression(bool isOperator)
    : isOperator(isOperator) {
    }

    MamdaniExpression::~MamdaniExpression() {
    }

    MamdaniProposition::MamdaniProposition()
    : MamdaniExpression(false), variable(NULL), term(NULL) {
    }

    std::string MamdaniProposition::toString() const {
        std::stringstream ss;
        ss << variable->getName() << " " << Rule::FL_IS << " ";
        for (std::size_t i = 0; i < hedges.size(); ++i) {
            ss << hedges.at(i)->name() << " ";
        }
        ss << term->getName();
        return ss.str();
    }

    MamdaniOperator::MamdaniOperator()
    : MamdaniExpression(true), left(NULL), right(NULL) {
    }

    MamdaniOperator::~MamdaniOperator() {
        if (left)
            delete left;
        if (right)
            delete right;
    }

    std::string MamdaniOperator::toString() const {
        return name;
    }

}
