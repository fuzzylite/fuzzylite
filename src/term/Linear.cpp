/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/term/Linear.h"

#include "fl/variable/InputVariable.h"

namespace fl {

    Linear::Linear(const std::string& name,
            const std::vector<scalar>& coefficients,
            const Engine* engine)
    : Term(name), _coefficients(coefficients), _engine(engine) { }

    Linear::~Linear() { }

    std::string Linear::className() const {
        return "Linear";
    }

    Complexity Linear::complexity() const {
        Complexity result;
        result.comparison(1 + 1);
        if (_engine) {
            result.arithmetic(scalar(_engine->variables().size()));
            result.comparison(scalar(_engine->variables().size())); //if (i < coefficients)
        }
        return result;
    }

    scalar Linear::membership(scalar x) const {
        FL_IUNUSED(x);
        if (not _engine)
            throw Exception("[linear error] term <" + getName() + "> "
                "is missing a reference to the engine", FL_AT);

        scalar result = 0.0;
        const std::size_t numberOfInputVariables = _engine->inputVariables().size();
        const std::size_t numberOfCoefficients = _coefficients.size();
        for (std::size_t i = 0; i < numberOfInputVariables; ++i) {
            if (i < numberOfCoefficients)
                result += _coefficients.at(i) * _engine->inputVariables().at(i)->getValue();
        }
        if (numberOfCoefficients > numberOfInputVariables) {
            result += _coefficients.back();
        }
        return result;
    }

    void Linear::set(const std::vector<scalar>& coefficients, const Engine* engine) {
        setCoefficients(coefficients);
        setEngine(engine);
    }

    void Linear::setCoefficients(const std::vector<scalar>& coefficients) {
        this->_coefficients = coefficients;
    }

    const std::vector<scalar>& Linear::coefficients() const {
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
        this->_coefficients.clear();
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

    void Linear::updateReference(const Engine* engine) {
        setEngine(engine);
    }

    Term* Linear::constructor() {
        return new Linear;
    }

}
