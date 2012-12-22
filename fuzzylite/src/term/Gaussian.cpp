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
            scalar mean, scalar sigma)
    : Term(name), _mean(mean), _sigma(sigma) {
    }

    Gaussian::~Gaussian() {
    }

    std::string Gaussian::className() const {
        return "Gaussian";
    }

    Gaussian* Gaussian::copy() const {
        return new Gaussian(*this);
    }

    scalar Gaussian::membership(scalar x) const {
        return std::exp((-(x - _mean) * (x - _mean)) / (2 * _sigma * _sigma));
    }

    std::string Gaussian::toString() const {
        std::stringstream ss;
        ss << className() << " (" << _mean << ", " << _sigma << ")";
        return ss.str();
    }

    void Gaussian::setMean(scalar c) {
        this->_mean = c;
    }

    scalar Gaussian::getMean() const {
        return this->_mean;
    }

    void Gaussian::setSigma(scalar sigma) {
        this->_sigma = sigma;
    }

    scalar Gaussian::getSigma() const {
        return this->_sigma;
    }

}
