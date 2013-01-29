/*
 * scalar.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_SCALAR_H
#define FL_SCALAR_H

namespace fl {
#ifdef FL_USE_FLOAT
    typedef float scalar;
#else
    typedef double scalar;
#endif

}

#endif /* FL_SCALAR_H */
