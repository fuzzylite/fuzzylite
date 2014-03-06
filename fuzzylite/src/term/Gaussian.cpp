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

/*
 * Gaussian.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "fl/term/Gaussian.h"

#include <cmath>
#include <sstream>

namespace fl {

    Gaussian::Gaussian(const std::string& name,
            scalar mean, scalar standardDeviation)
    : Term(name), _mean(mean), _standardDeviation(standardDeviation) {
    }

    Gaussian::~Gaussian() {
    }

    std::string Gaussian::className() const {
        return "Gaussian";
    }

    scalar Gaussian::membership(scalar x) const {
        if (fl::Op::isNaN(x)) return fl::nan;
        return std::exp((-(x - _mean) * (x - _mean)) / (2 * _standardDeviation * _standardDeviation));
    }

    std::string Gaussian::parameters() const {
        return Op::join(2, " ", _mean, _standardDeviation);
    }

    void Gaussian::configure(const std::string& parameters) {
        if (parameters.empty()) return;
        std::vector<std::string> values = Op::split(parameters, " ");
        std::size_t required = 2;
        if (values.size() < required) {
            std::ostringstream ex;
            ex << "[configuration error] term <" << className() << ">"
                    << " requires <" << required << "> parameters";
            throw fl::Exception(ex.str(), FL_AT);
        }
        setMean(Op::toScalar(values.at(0)));
        setStandardDeviation(Op::toScalar(values.at(1)));
    }

    void Gaussian::setMean(scalar c) {
        this->_mean = c;
    }

    scalar Gaussian::getMean() const {
        return this->_mean;
    }

    void Gaussian::setStandardDeviation(scalar sigma) {
        this->_standardDeviation = sigma;
    }

    scalar Gaussian::getStandardDeviation() const {
        return this->_standardDeviation;
    }

    Gaussian* Gaussian::copy() const {
        return new Gaussian(*this);
    }

    Term* Gaussian::constructor() {
        return new Gaussian;
    }

}
