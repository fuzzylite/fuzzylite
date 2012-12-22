#include "fl/rule/mamdani/MamdaniExpression.h"

namespace fl {

    MamdaniExpression::MamdaniExpression(bool isOperator)
    : isOperator(isOperator) {
    }

    MamdaniExpression::~MamdaniExpression() {
    }

    MamdaniAntecedentProposition::MamdaniAntecedentProposition()
    : MamdaniExpression(false), inputVariable(NULL), term(NULL) {
    }

    std::string MamdaniAntecedentProposition::toString() const {
        std::stringstream ss;
        ss << inputVariable->getName() << " " << Rule::FL_IS << " ";
        for (std::size_t i = 0; i < hedges.size(); ++i) {
            ss << hedges[i]->name();
            if (i < hedges.size() - 1)
                ss << " ";
        }
        ss << term->getName();
        return ss.str();
    }

    MamdaniConsequentProposition::MamdaniConsequentProposition()
    : MamdaniExpression(false), outputVariable(NULL), term(NULL), weight(1.0) {
    }

    std::string MamdaniConsequentProposition::toString() const {
        std::stringstream ss;
        ss << outputVariable->getName() << " " << Rule::FL_IS << " ";
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
