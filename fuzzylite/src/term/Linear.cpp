// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE


#include "fl/term/Linear.h"

#include <cstdarg>

namespace fl {

    Linear::Linear(const std::string& name,
            const std::vector<scalar>& coefficients,
            const std::vector<InputVariable*>& inputVariables)
    : Term(name), coefficients(coefficients) {
        this->inputVariables = std::vector<const InputVariable*>
                (inputVariables.begin(), inputVariables.end());
    }

    Linear::~Linear() {
    }

    std::string Linear::className() const {
        return "Linear";
    }

    scalar Linear::membership(scalar x) const {
        (void) x;
        if (coefficients.size() != inputVariables.size() + 1) {
            std::ostringstream ss;
            ss << "[linear error] the number of coefficients <" << coefficients.size() << "> "
                    "need to be equal to the number of input variables "
                    "<" << inputVariables.size() << "> plus a constant c "
                    "(e.g. ax + by + c)";
            throw fl::Exception(ss.str(), FL_AT);
        }
        scalar result = 0;
        for (std::size_t i = 0; i < inputVariables.size(); ++i) {
            result += coefficients.at(i) * inputVariables.at(i)->getInputValue();
        }
        if (coefficients.size() > inputVariables.size()) {
            result += coefficients.back();
        }

        return result;
    }

    std::string Linear::parameters() const {
        return Op::join(this->coefficients, " ");
    }

    void Linear::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> strValues = Op::split(parameters, " ");
        std::vector<scalar> values;
        for (std::size_t i = 0; i < strValues.size(); ++i) {
            values.push_back(Op::toScalar(strValues.at(i)));
        }
        this->coefficients = values;
    }

    template <typename T>
    Linear* Linear::create(const std::string& name,
            const std::vector<InputVariable*>& inputVariables,
            T firstCoefficient, ...) {
        std::vector<scalar> coefficients;
        coefficients.push_back(firstCoefficient);

        va_list args;
        va_start(args, firstCoefficient);
        for (std::size_t i = 0; i < inputVariables.size(); ++i) {
            coefficients.push_back((scalar) va_arg(args, T));
        }
        va_end(args);
        return new Linear(name, coefficients, inputVariables);
    }

    template FL_EXPORT Linear* Linear::create(const std::string& name,
            const std::vector<InputVariable*>& inputVariables,
            double firstCoefficient, ...);

    template FL_EXPORT Linear* Linear::create(const std::string& name,
            const std::vector<InputVariable*>& inputVariables,
            int firstCoefficient, ...);

    void Linear::set(const std::vector<scalar>& coefficients,
            const std::vector<InputVariable*>& inputVariables) throw (fl::Exception) {
        set(coefficients, std::vector<const InputVariable*>
                (inputVariables.begin(), inputVariables.end()));
    }

    void Linear::set(const std::vector<scalar>& coefficients,
            const std::vector<const InputVariable*>& inputVariables) throw (fl::Exception) {
        if (coefficients.size() != inputVariables.size() + 1) {
            std::ostringstream ss;
            ss << "[linear term] the number of coefficients <" << coefficients.size() << "> "
                    "need to be equal to the number of input variables "
                    "<" << inputVariables.size() << "> plus a constant c "
                    "(i.e. ax + by + c)";
            throw fl::Exception(ss.str(), FL_AT);
        }
        this->coefficients = coefficients;
        this->inputVariables = inputVariables;
    }

    Linear* Linear::copy() const {
        return new Linear(*this);
    }

    Term* Linear::constructor() {
        return new Linear;
    }
}
