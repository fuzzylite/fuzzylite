

#include "fl/term/Linear.h"

namespace fl {

    Linear::Linear(const std::string& name = "")
    : Term(name) { }

    Linear::~Linear() { }

    std::string Linear::className() const {
        return "Linear";
    }

    Linear* Linear::copy() const {
        return new Linear(this);
    }

    scalar Linear::membership(scalar x) const {
        (void) x;
        std::map<std::string, std::pair<scalar, const InputVariable*> >::const_iterator it =
                this->_coefficients.begin();
        scalar result = 0.0;
        while (it != this->_coefficients.end()) {
            std::pair<scalar, const InputVariable*> weightVariable = it->second;
            result += weightVariable.first * weightVariable.second->getInput();
            it++;
        }
        return result;
    }

    std::string Linear::toString() const {
        std::map<std::string, std::pair<scalar, const InputVariable*> >::const_iterator it =
                this->_coefficients.begin();
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (";
        while (it != this->_coefficients.end()) {
            std::pair<scalar, const InputVariable*> weightVariable = it->second;
            ss << weightVariable.first << "*" << weightVariable.second->getName();
            it++;
            if (it != this->_coefficients.end()) {
                ss << ", ";
            }
        }
        ss << ")";
        return ss.str();
    }

    void Linear::setCoefficient(scalar coefficient, const InputVariable* input) {
        this->_coefficients.at(input->getName()) =
                std::pair<scalar, const InputVariable*>(coefficient, input);
    }

    scalar Linear::getCoefficient(const std::string& inputVariable) const {
        std::map<std::string, std::pair<scalar, const InputVariable*> >::const_iterator it =
                this->_coefficients.find(inputVariable);
        if (it == this->_coefficients.end()) return std::numeric_limits<scalar>::quiet_NaN();
        return it->second->first;
    }

    scalar Linear::removeCoefficient(const std::string& inputVariable) {
        std::map<std::string, std::pair<scalar, const InputVariable*> >::iterator it =
                this->_coefficients.find(inputVariable);
        if (it == this->_coefficients.end()) return std::numeric_limits<scalar>::quiet_NaN();
        scalar result = it->second->first;
        this->_coefficients.erase(it);
        return result;
    }

    bool Linear::hasCoefficient(const std::string& inputVariable) const {
        std::map<std::string, std::pair<scalar, const InputVariable*> >::iterator it =
                this->_coefficients.find(inputVariable);
        return not this->_coefficients.end();
    }

    void Linear::clearCoefficients() {
        this->_coefficients.clear();
    }

}
