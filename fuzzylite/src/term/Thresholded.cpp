/*
 * Thresholded.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "fl/term/Thresholded.h"

#include "fl/operator/TNorm.h"


#include <sstream>

namespace fl {

    Thresholded::Thresholded(const Term* term, scalar threshold, const TNorm* activation)
    : Term(""), _term(term), _threshold(threshold), _activation(activation) {
        if (term) this->_name = term->getName();
    }

    Thresholded::~Thresholded() {
    }

    std::string Thresholded::className() const {
        return "Thresholded";
    }

    Thresholded* Thresholded::copy() const {
        return new Thresholded(*this);
    }

    scalar Thresholded::membership(scalar x) const {
        if (fl::Op::isNan(x)) return std::numeric_limits<scalar>::quiet_NaN();
        return _activation->compute(this->_term->membership(x), _threshold);
    }

    std::string Thresholded::toString() const {
        std::ostringstream ss;
        ss << std::setprecision(FL_DECIMALS) << std::fixed;
        ss << className() << " (" << _term->toString() << ") thresholded to " << _threshold
                << " using <" << _activation->className() << "> activation";
        return ss.str();
    }

    void Thresholded::setTerm(const Term* term) {
        this->_term = term;
    }

    const Term* Thresholded::getTerm() const {
        return this->_term;
    }

    void Thresholded::setThreshold(scalar threshold) {
        this->_threshold = threshold;
    }

    scalar Thresholded::getThreshold() const {
        return this->_threshold;
    }

    void Thresholded::setActivation(const TNorm* activation) {
        this->_activation = activation;
    }

    const TNorm* Thresholded::getActivation() const {
        return this->_activation;
    }

}
