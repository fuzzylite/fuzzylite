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

Bell::Bell(const std::string& name, scalar minimum, scalar maximum, scalar a,
		scalar b, scalar c) :
		Term(name, minimum, maximum), _a(a), _b(b), _c(c) {

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

} /* namespace fl */
