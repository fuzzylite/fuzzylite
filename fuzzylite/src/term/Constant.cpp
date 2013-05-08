#include "fl/term/Constant.h"

namespace fl {

    Constant::Constant(const std::string& name, scalar value)
    : Term(name), _value(value) { }

    Constant::~Constant() { }

    std::string Constant::className() const {
        return "Constant";
    }

    scalar Constant::membership(scalar x) const {
//        return fl::Op::isEq(x, this->_value);
        return this->_value;
    }

    Constant* Constant::copy() const {
        return new Constant(*this);
    }

    std::string Constant::toString() const {
        std::ostringstream ss;
        ss << className() << "(" << this->_value << ")";
        return ss.str();
    }

    void Constant::setValue(scalar value) {
        this->_value = value;
    }

    scalar Constant::getValue() const {
        return this->_value;
    }

}
