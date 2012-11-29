/*
 * Shoulder.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_SHOULDER_H_
#define FL_SHOULDER_H_

#include "../Term.h"

namespace fl {

class LeftShoulder: public Term {
public:
	LeftShoulder(const std::string& name,
			scalar minimum = -std::numeric_limits<scalar>::infinity(),
			scalar maximum = std::numeric_limits<scalar>::infinity());
	~LeftShoulder();

	scalar membership(scalar x);
	std::string toString() const;
};

class RightShoulder: public Term {
public:
	RightShoulder(const std::string& name,
			scalar minimum = -std::numeric_limits<scalar>::infinity(),
			scalar maximum = std::numeric_limits<scalar>::infinity());
	~RightShoulder();

	scalar membership(scalar x);
	std::string toString() const;
};



} /* namespace fl */
#endif /* FL_SHOULDER_H_ */
