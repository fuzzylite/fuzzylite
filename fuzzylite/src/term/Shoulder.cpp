/*
 * Shoulder.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include "Shoulder.h"

#include <sstream>

namespace fl {

/**
 * Left Shoulder
 */
LeftShoulder::LeftShoulder(const std::string& name, scalar minimum, scalar maximum)
: Term(name, minimum, maximum){

}

LeftShoulder::~LeftShoulder() {

}

scalar LeftShoulder::membership(scalar  x){
	if (x <= _minimum) return 1.0;
	if (x >= _maximum) return 0.0;
	return (x - _minimum) / (_maximum - _minimum);
}

std::string LeftShoulder::toString() const{
	std::stringstream ss;
	ss << "LeftShoulder (" << _minimum << ", " << _maximum << ")";
	return ss.str();
}


/**
 * Right Shoulder
 */
RightShoulder::RightShoulder(const std::string& name, scalar minimum, scalar maximum)
: Term(name, minimum, maximum){

}

RightShoulder::~RightShoulder() {

}

scalar RightShoulder::membership(scalar  x){
	if (x <= _minimum) return 0.0;
	if ( x >= _maximum) return 1.0;
	return (_maximum - x) / (_maximum - _minimum);
}

std::string RightShoulder::toString() const{
	std::stringstream ss;
	ss << "RightShoulder (" << _minimum << ", " << _maximum << ")";
	return ss.str();
}




} /* namespace fl */
