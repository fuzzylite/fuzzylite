/*
 * Rectangle.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Rectangle.h"

#include <sstream>

namespace fl {

Rectangle::Rectangle(const std::string& name, scalar minimum, scalar maximum) :
		Term(name, minimum, maximum) {

}

Rectangle::~Rectangle() {

}

scalar Rectangle::membership(scalar x){
	if (x < _minimum || x > _maximum) return 0.0;
	return 1.0;
}

std::string Rectangle::toString() const{
	std::stringstream ss;
	ss << "Rectangle (" << _minimum << ", " << _maximum << ")";
	return ss.str();
}

} /* namespace fl */
