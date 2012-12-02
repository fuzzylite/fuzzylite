/*
 * CenterOfGravity.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "CenterOfGravity.h"

#include "../term/Term.h"

namespace fl {

CenterOfGravity::CenterOfGravity(int divisions)
		: Defuzzifier(divisions) {
}

std::string CenterOfGravity::name() const {
	return "COG";
}

scalar CenterOfGravity::defuzzify(const Term* term) const {
	scalar dx = (term->maximum() - term->minimum()) / _divisions;
	scalar x, y;
	scalar area = 0, xcentroid = 0, ycentroid = 0;
	for (int i = 0; i < _divisions; ++i) {
		x = term->minimum() + (i + 0.5) * dx;
		y = term->membership(x);

		xcentroid += y * x;
		ycentroid += y * y;
		area += y;
	}
	xcentroid /= area;
	ycentroid /= 2 * area;
	area *= dx; //total area... unused, but for future reference.
	return xcentroid;
}

} /* namespace fl */
