

#include "fl/term/Linear.h"

#include <cstdarg>

namespace fl {

    Linear::Linear(const std::string& name,
            const std::vector<scalar>& coefficients,
            const std::vector<InputVariable*>& inputVariables)
    : Term(name), _coefficients(coefficients) {
        _inputVariables = std::vector<const InputVariable*>(inputVariables.begin(), inputVariables.end());
    }

    Linear::Linear(const std::string& name, const std::vector<InputVariable*>& inputVariables,
            int argc, ...) throw (fl::Exception)
    : Term(name) {
        _inputVariables = std::vector<const InputVariable*>(inputVariables.begin(), inputVariables.end());
        if (not (argc == (int) _inputVariables.size() or
                argc == (int) _inputVariables.size() + 1)) {
            std::ostringstream ss;
            ss << "[term error] linear term expects the number of "
                    "coefficients <" << argc << "> to be equal or greater by one "
                    "to the number of input variables <" << _inputVariables.size() << ">";
            throw fl::Exception(ss.str(), FL_AT);
        }
        va_list args;
        va_start(args, argc);
        for (int i = 0; i < argc; ++i) {
            _coefficients.push_back((scalar) va_arg(args, double));
        }
        va_end(args);
    }

    Linear::~Linear() { }

    std::string Linear::className() const {
        return "Linear";
    }

    Linear* Linear::copy() const {
        return new Linear(*this);
    }

    scalar Linear::membership(scalar x) const {
        (void) x;
        if (not (_coefficients.size() == _inputVariables.size() or
                _coefficients.size() == _inputVariables.size() + 1)) {
            std::ostringstream ss;
            ss << "[term error] the number of coefficients "
                    "(" << _coefficients.size() << ") must match the number "
                    "of input variables (" << _inputVariables.size() << ") or "
                    "exceed it by one";
            throw fl::Exception(ss.str(), FL_AT);
        }
        scalar result = 0;
        for (std::size_t i = 0; i < _inputVariables.size(); ++i) {
            result += _coefficients.at(i) * _inputVariables.at(i)->getInput();
        }
        if (_coefficients.size() > _inputVariables.size()) {
            result += _coefficients.back();
        }

        return result;
    }

    std::string Linear::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (";
        for (std::size_t i = 0; i < _coefficients.size(); ++i) {
            ss << _coefficients.at(i);
            if (i < _coefficients.size() - 1) {
                ss << ", ";
            }
        }
        ss << ")";

        return ss.str();
    }

    void Linear::setInputVariables(const std::vector<InputVariable*>& inputVariables) {
        this->_inputVariables = std::vector<const InputVariable*>(
                inputVariables.begin(),
                inputVariables.end());
    }

    void Linear::setInputVariables(const std::vector<const InputVariable*>& inputVariables) {
        this->_inputVariables = inputVariables;
    }

    const std::vector<const InputVariable*>& Linear::getInputVariables() const {
        return this->_inputVariables;
    }

    void Linear::setCoefficients(const std::vector<scalar>& coefficients) {
        this->_coefficients = coefficients;
    }

    const std::vector<scalar>& Linear::getCoefficients() const {
        return this->_coefficients;
    }

    void Linear::setNumberOfCoefficients(int coefficients) {
        this->_coefficients = std::vector<scalar>(coefficients);
    }

    int Linear::getNumberOfCoefficients() const {
        return this->_coefficients.size();
    }

    void Linear::setCoefficient(int index, scalar coefficient) {
        this->_coefficients.at(index) = coefficient;
    }

    scalar Linear::getCoefficient(int index) const {
        return this->_coefficients.at(index);
    }

}
