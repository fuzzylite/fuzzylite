
#include "fl/term/Weighted.h"

namespace fl {

    Weighted::Weighted(const std::string& name, const Term* term, scalar weight)
    : Term(name), _term(term), _weight(weight) { }

    Weighted::~Weighted() { }

    void Weighted::setTerm(const Term* term) {
        this->_term = term;
    }

    const Term* Weighted::getTerm() const {
        return this->_term;
    }

    void Weighted::setWeight(scalar weight) {
        this->_weight = weight;
    }

    scalar Weighted::getWeight() const {
        return this->_weight;
    }

    scalar Weighted::membership(scalar x) const {
        return this->_weight * this->_term->membership(x);
    }

    std::string Weighted::className() const {
        return "Weighted";
    }

    std::string Weighted::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (";
        ss << _term->toString();
        ss << ") using " << this->_weight;
        return ss.str();
    }

    Weighted* Weighted::copy() const {
        return new Weighted(this);
    }

}
