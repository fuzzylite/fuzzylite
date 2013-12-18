/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

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

    scalar Gaussian::membership(scalar x) const {
        if (fl::Op::isNan(x)) return fl::nan;
        return std::exp((-(x - _mean) * (x - _mean)) / (2 * _standardDeviation * _standardDeviation));
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
