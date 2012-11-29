/*
 * Test.cpp
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#include <limits>
#include "scalar.h"
#include "definitions.h"

using namespace fl;
int main(){

	FL_LOG("Test");
	scalar x = std::numeric_limits<scalar>::infinity();
	FL_LOG( (98.0 < x) );
	FL_LOG( (98.0 > x) );
	x = -std::numeric_limits<scalar>::infinity();
	FL_LOG( (98.0 < x) );
	FL_LOG( (98.0 > x) );

}

