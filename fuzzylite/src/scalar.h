/*
 * scalar.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_SCALAR_H_
#define FL_SCALAR_H_

namespace fl {
#ifdef FL_USE_SINGLE_PRECISION
    typedef float scalar;
#define FL_EPSILON 1e-5

#else
    typedef double scalar;
#define FL_EPSILON 1e-5
#endif

}

#endif /* FL_SCALAR_H_ */
