/*
 * Bell.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Bell.h"

#include <cmath>
#include <sstream>

namespace fl {

Bell::Bell(const std::string& name, scalar a, scalar b, scalar c,
		scalar minimum, scalar maximum)
		: Term(name), _a(a), _b(b), _c(c), _minimum(minimum), _maximum(maximum) {
}

Bell::~Bell() {
}

scalar Bell::membership(scalar x) {
	// from matlab: gbellmf.m
	scalar tmp = ((x - _c) / _a) * ((x - _c) / _a);
	if (tmp == 0.0 && _b == 0)
	return 0.5;
	else if (tmp == 0.0 && _b < 0)
	return 0.0;
	else {
		tmp = std::pow(tmp, _b);
		return 1.0 / (1.0 + tmp);
	}
}

std::string Bell::toString() const {
	std::stringstream ss;
	ss << "Bell (" << _minimum << ", " << _maximum << ", " << _a << ", " << _b
			<< ", " << _c << ")";
	return ss.str();
}

void Bell::setA(scalar a) {
	this->_a = a;
}
scalar Bell::getA() const {
	return this->_a;
}
void Bell::setB(scalar b) {
	this->_b = b;
}
scalar Bell::getB() const {
	return this->_b;
}
void Bell::setC(scalar c) {
	this->_c = c;
}
scalar Bell::getC() const {
	return this->_c;
}

void Bell::setMinimum(scalar minimum) {
	this->_minimum = minimum;
}
scalar Bell::minimum() const {
	return this->_minimum;
}
void Bell::setMaximum(scalar maximum) {
	this->_maximum = maximum;
}
scalar Bell::maximum() const {
	return this->_maximum;
}

} /* namespace fl */
