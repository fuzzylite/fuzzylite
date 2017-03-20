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

#include "fl/term/Gaussian.h"

namespace fl {

    Gaussian::Gaussian(const std::string& name,
            scalar mean, scalar standardDeviation, scalar height)
    : Term(name, height), _mean(mean), _standardDeviation(standardDeviation) { }

    Gaussian::~Gaussian() { }

    std::string Gaussian::className() const {
        return "Gaussian";
    }

    Complexity Gaussian::complexity() const {
        return Complexity().comparison(1).arithmetic(7).function(1);
    }

    scalar Gaussian::membership(scalar x) const {
        if (Op::isNaN(x)) return fl::nan;
        return Term::_height * std::exp((-(x - _mean) * (x - _mean)) / (2.0 * _standardDeviation * _standardDeviation));
    }

    std::string Gaussian::parameters() const {
        return Op::join(2, " ", _mean, _standardDeviation) +
                (not Op::isEq(getHeight(), 1.0) ? " " + Op::str(getHeight()) : "");
    }

    void Gaussian::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw Exception(ex.str(), FL_AT);
        }
        setMean(Op::toScalar(values.at(0)));
        setStandardDeviation(Op::toScalar(values.at(1)));
        if (values.size() > required)
            setHeight(Op::toScalar(values.at(required)));
    }

    void Gaussian::setMean(scalar mean) {
        this->_mean = mean;
    }

    scalar Gaussian::getMean() const {
        return this->_mean;
    }

    void Gaussian::setStandardDeviation(scalar standardDeviation) {
        this->_standardDeviation = standardDeviation;
    }

    scalar Gaussian::getStandardDeviation() const {
        return this->_standardDeviation;
    }

    Gaussian* Gaussian::clone() const {
        return new Gaussian(*this);
    }

    Term* Gaussian::constructor() {
        return new Gaussian;
    }


}
