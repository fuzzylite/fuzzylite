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

#include "fl/Engine.h"

#include <cstdarg>
#include <memory>

namespace fl {

    Linear::Linear(const std::string& name,
            const std::vector<scalar>& coefficients,
            const Engine* engine)
    : Term(name), _coefficients(coefficients), _engine(engine) {
    }

    Linear::~Linear() {
    }

    std::string Linear::className() const {
        return "Linear";
    }

    scalar Linear::membership(scalar x) const {
        (void) x;
        if (not _engine) {
            throw fl::Exception("[linear error] term <" + getName() + "> "
                    "requires a reference to the engine, but none was set", FL_AT);
        }
        if (_coefficients.size() != _engine->constInputVariables().size() + 1) {
            std::ostringstream ss;
            ss << "[linear error] the number of coefficients <" << _coefficients.size() << "> "
                    " in term <" << getName() << "> needs to be equal to the number of input variables "
                    "<" << _engine->constInputVariables().size() << "> plus a constant c "
                    "(e.g. ax + by + c)";
            throw fl::Exception(ss.str(), FL_AT);
        }
        scalar result = 0;
        for (std::size_t i = 0; i < _engine->constInputVariables().size(); ++i) {
            result += _coefficients.at(i) * _engine->constInputVariables().at(i)->getInputValue();
        }
        if (_coefficients.size() > _engine->constInputVariables().size()) {
            result += _coefficients.back();
        }

        return result;
    }

    void Linear::set(const std::vector<scalar>& coeffs, const Engine* engine) throw (fl::Exception) {
        if (not _engine) {
            throw fl::Exception("[linear error] term <" + getName() + "> "
                    "requires a reference to the engine, but none was set", FL_AT);
        }
        if (coeffs.size() != _engine->constInputVariables().size() + 1) {
            std::ostringstream ss;
            ss << "[linear error] the number of coefficients <" << coeffs.size() << "> "
                    " in term <" << getName() << "> needs to be equal to the number of input variables "
                    "<" << _engine->constInputVariables().size() << "> plus a constant c "
                    "(e.g. ax + by + c)";
            throw fl::Exception(ss.str(), FL_AT);
        }
        this->_coefficients = coeffs;
        this->_engine = engine;
    }

    void Linear::setCoefficients(const std::vector<scalar>& coeffs) {
        this->_coefficients = coeffs;
    }

    const std::vector<scalar>& Linear::constCoefficients() const {
        return this->_coefficients;
    }

    std::vector<scalar>& Linear::coefficients() {
        return this->_coefficients;
    }

    void Linear::setEngine(const Engine* engine) {
        this->_engine = engine;
    }

    const Engine* Linear::getEngine() const {
        return this->_engine;
    }

    std::string Linear::parameters() const {
        return Op::join(this->_coefficients, " ");
    }

    void Linear::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> strValues = Op::split(parameters, " ");
        std::vector<scalar> values;
        for (std::size_t i = 0; i < strValues.size(); ++i) {
            values.push_back(Op::toScalar(strValues.at(i)));
        }
        this->_coefficients = values;
    }

    Linear* Linear::clone() const {
        return new Linear(*this);
    }

    Term* Linear::constructor() {
        return new Linear;
    }

    template <typename T>
    Linear* Linear::create(const std::string& name,
            const Engine* engine, T firstCoefficient, ...) throw (fl::Exception) {
        if (not engine) {
            throw fl::Exception("[linear error] cannot create term <" + name + "> "
                    "without a reference to the engine", FL_AT);
        }
        std::vector<scalar> coefficients;
        coefficients.push_back(firstCoefficient);

        va_list args;
        va_start(args, firstCoefficient);
        for (std::size_t i = 0; i < engine->constInputVariables().size(); ++i) {
            coefficients.push_back((scalar) va_arg(args, T));
        }
        va_end(args);

        std::auto_ptr<Linear> result(new Linear(name));
        result->set(coefficients, engine);
        return result.release();
    }

    template FL_EXPORT Linear* Linear::create(const std::string& name,
            const Engine* engine,
            double firstCoefficient, ...);

    template FL_EXPORT Linear* Linear::create(const std::string& name,
            const Engine* engine,
            int firstCoefficient, ...);
}
