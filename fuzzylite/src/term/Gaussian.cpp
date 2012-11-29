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

Gaussian::Gaussian(const std::string& name, scalar minimum, scalar maximum,
		scalar sigma, scalar c)
: Term(name, minimum, maximum), _sigma(sigma), _c(c){

}

Gaussian::~Gaussian() {

}

scalar Gaussian::membership(scalar x){
	return std::exp((-(x - _c) * (x - _c)) / (2 * _sigma * _sigma));
}

std::string Gaussian::toString() const{
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


} /* namespace fl */
