/*
 * Thresholded.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "Thresholded.h"

#include "../engine/Operator.h"

#include <sstream>

namespace fl {

Thresholded::Thresholded(const std::string& name, const Term* term, scalar threshold, const Operator* activationOperator)
		: Term(name), _term(term), _threshold(threshold), _activationOperator(activationOperator) {
}

Thresholded::~Thresholded() {
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

void Thresholded::setActivationOperator(const Operator* activationOperator) {
	this->_activationOperator = activationOperator;
}

const Operator* Thresholded::getActivationOperator() const {
	return this->_activationOperator;
}

std::string Thresholded::toString() const {
	std::stringstream ss;
	ss << "Thresholded (" << _term->toString() << ") at " << _threshold
			<< " with " << _activationOperator->name();
	return ss.str();
}

scalar Thresholded::minimum() const {
	return this->_term->minimum();
}

scalar Thresholded::maximum() const {
	return this->_term->maximum();
}

} /* namespace fl */
