#include "InputLVar.h"

namespace fuzzy_lite {

    InputLVar::InputLVar() : LinguisticVariable(), _input(flScalar(0.0)) {

    }

    InputLVar::InputLVar(const std::string& name) : LinguisticVariable(name),
    _input(flScalar(0.0)) {

    }

    InputLVar::~InputLVar() {
    }

    void InputLVar::setInput(flScalar input) {
        this->_input = input;
    }

    flScalar InputLVar::getInput() const {
        return this->_input;
    }
}
