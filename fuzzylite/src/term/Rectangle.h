/*
 * Rectangle.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_RECTANGLE_H_
#define FL_RECTANGLE_H_

#include "../Term.h"

namespace fl {

class Rectangle: public Term {
public:
	Rectangle(const std::string& name,
			scalar minimum = -std::numeric_limits<scalar>::infinity(),
			scalar maximum = std::numeric_limits<scalar>::infinity());
	~Rectangle();

	scalar membership(scalar x);

	std::string toString() const;
};

} /* namespace fl */
#endif /* FL_RECTANGLE_H_ */
