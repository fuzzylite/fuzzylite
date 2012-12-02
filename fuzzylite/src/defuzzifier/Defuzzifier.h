/*
 * Defuzzifier.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_DEFUZZIFIER_H_
#define FL_DEFUZZIFIER_H_

#include "../scalar.h"

#include <string>

#define FL_DEFAULT_DIVISIONS 100

namespace fl {
class Term;

class Defuzzifier {
	protected:
		int _divisions;

	public:
		Defuzzifier(int divisions = FL_DEFAULT_DIVISIONS)
				: _divisions(divisions) {
		}
		virtual ~Defuzzifier() {
		}

		virtual std::string name() const = 0;
		virtual scalar defuzzify(const Term* term) const = 0;

		virtual void setDivisions(int divisions) {
			this->_divisions = divisions;
		}
		virtual int getDivisions() const {
			return this->_divisions;
		}

};

} /* namespace fl */
#endif /* FL_DEFUZZIFIER_H_ */
