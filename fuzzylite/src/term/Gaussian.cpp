/*
 * Gaussian.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Gaussian.h"

#include <cmath>
#include <sstream>

namespace fl {

    Gaussian::Gaussian(const std::string& name,
            scalar sigma, scalar c, scalar minimum, scalar maximum)
            : Term(name), _sigma(sigma), _c(c), _minimum(minimum), _maximum(
                    maximum) {
    }

    Gaussian::~Gaussian() {

    }

    scalar Gaussian::membership(scalar x) const{
        return std::exp((-(x - _c) * (x - _c)) / (2 * _sigma * _sigma));
    }

    std::string Gaussian::toString() const {
        std::stringstream ss;
        ss << "Gaussian (" << _minimum << ", " << _maximum << ", "
                << _sigma << ", " << _c << ")";
        return ss.str();
    }

    void Gaussian::setSigma(scalar sigma) {
        this->_sigma = sigma;
    }
    scalar Gaussian::getSigma() const {
        return this->_sigma;
    }

    void Gaussian::setC(scalar c) {
        this->_c = c;
    }
    scalar Gaussian::getC() const {
        return this->_c;
    }

    void Gaussian::setMinimum(scalar minimum) {
        this->_minimum = minimum;
    }
    scalar Gaussian::minimum() const {
        return this->_minimum;
    }
    void Gaussian::setMaximum(scalar maximum) {
        this->_maximum = maximum;
    }
    scalar Gaussian::maximum() const {
        return this->_maximum;
    }

} /* namespace fl */
