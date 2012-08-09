#include "FuzzyProposition.h"
#include "MamdaniRule.h"
#include "FuzzyExceptions.h"
#include "StringOperator.h"

#include <sstream>


namespace fuzzy_lite {

    FuzzyProposition::FuzzyProposition() :
    _left(NULL), _right(NULL), _operation(O_NONE),
    _input_lvar(NULL), _term(NULL) {
    }

    FuzzyProposition::~FuzzyProposition() {
        if (_left) {
            delete _left;
        }
        if (_right) {
            delete _right;
        }
    }

    void FuzzyProposition::setLeft(FuzzyProposition& proposition) {
        this->_left = &proposition;
    }

    void FuzzyProposition::setRight(FuzzyProposition& proposition) {
        this->_right = &proposition;
    }

    void FuzzyProposition::setOperation(OPERATION operation) {
        this->_operation = operation;
    }

    void FuzzyProposition::setInputLVar(const InputLVar& lvar) {
        this->_input_lvar = &lvar;
    }

    void FuzzyProposition::addHedge(Hedge& hedge) {
        this->_hedges.push_back(&hedge);
    }

    void FuzzyProposition::setTerm(const DiscreteTerm& term) {
        this->_term = &term;
    }

    bool FuzzyProposition::isTerminal() const {
        return this->_operation == O_NONE;
    }

    FL_INLINE flScalar FuzzyProposition::degreeOfTruth(const FuzzyOperator& fuzzy_op) const {
        if (!isTerminal()) {
            if (_left == NULL || _right == NULL) {
                throw NullPointerException(FL_AT,toString());
            }
            switch (_operation) {
                case O_AND:
                    return fuzzy_op.fuzzyAnd(_left->degreeOfTruth(fuzzy_op), _right->degreeOfTruth(fuzzy_op));
                case O_OR:
                    return fuzzy_op.fuzzyOr(_left->degreeOfTruth(fuzzy_op), _right->degreeOfTruth(fuzzy_op));
                default:
                    throw InvalidArgumentException(FL_AT, "Operation " + StringOperator::IntToString(_operation) + " not available");
            }
        }
        //else
        flScalar result = _term->membership(_input_lvar->getInput());
        for (size_t i = 0; i < _hedges.size(); ++i) {
            result = _hedges[i]->hedge(result);
        }
        return result;
    }

    std::string FuzzyProposition::toString() const {
        std::stringstream ss;
        if (isTerminal()) {
            ss << _input_lvar->getName() + " " + MamdaniRule::FR_IS + " " + _term->getName();
        } else {
            ss << "(" + (_left ? _left->toString() : "NULL");
            ss << " " + (_operation == O_AND ? MamdaniRule::FR_AND : MamdaniRule::FR_OR);
            ss << " " + (_right ? _right->toString() : "NULL") + ")";
        }
        return ss.str();
    }
}
