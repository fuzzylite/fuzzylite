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
    : Term(name), _mean(mean), _standardDeviation(sigma) {
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
        return std::exp((-(x - _mean) * (x - _mean)) / (2 * _standardDeviation * _standardDeviation));
    }

    std::string Gaussian::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _mean << ", " << _standardDeviation << ")";
        return ss.str();
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

}
