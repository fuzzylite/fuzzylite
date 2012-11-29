/*
 * Sigmoid.cpp
 *
 *  Created on: 30/11/2012
 *      Author: jcrada
 */

#include "Sigmoid.h"

#include <cmath>
#include <sstream>

namespace fl {

Sigmoid::Sigmoid(const std::string& name, scalar minimum, scalar maximum,
		scalar a, scalar c)
:Term(name, minimum, maximum), _a(a), _c(c){

}

Sigmoid::~Sigmoid() {

}

scalar Sigmoid::membership(scalar x){
	return 1.0 / (1.0 + std::exp( -_a * (x - _c)));
}

std::string Sigmoid::toString() const{
	std::stringstream ss;
	ss << "Sigmoid (" << _minimum << ", " << _maximum << ", "
			<< _a << ", " << _c << ")";
	return ss.str();
}

void Sigmoid::setA(scalar a) {
	this->_a = a;
}
scalar Sigmoid::getA() const {
	return this->_a;
}
void Sigmoid::setC(scalar c) {
	this->_c = c;
}
scalar Sigmoid::getC() const {
	return this->_c;
}

} /* namespace fl */
