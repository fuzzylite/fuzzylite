/*
 * CenterOfGravity.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_CENTEROFGRAVITY_H_
#define FL_CENTEROFGRAVITY_H_

#include "Defuzzifier.h"

namespace fl {

class CenterOfGravity : public Defuzzifier{
	public:
		CenterOfGravity(int divisions = 100);

		std::string name() const;
		scalar defuzzify(const Term* term) const;

};

} /* namespace fl */
#endif /* FL_CENTEROFGRAVITY_H_ */
