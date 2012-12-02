/*
 * Consequent.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CONSEQUENT_H_
#define FL_CONSEQUENT_H_

#include "../scalar.h"

namespace fl {
class Operator;

class Consequent {
	public:
		Consequent() {
		}
		virtual ~Consequent() {
		}

		virtual void fire(scalar strength, const Operator* activation) = 0;

		virtual std::string toString() const = 0;

};

}

#endif /* FL_CONSEQUENT_H_ */
