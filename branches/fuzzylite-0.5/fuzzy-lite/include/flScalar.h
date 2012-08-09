/* 
 * File:   flScalar.h
 * Author: jcrada
 *
 * Created on October 30, 2009, 7:11 PM
 */

#ifndef _FLSCALAR_H
#define	_FLSCALAR_H

namespace fuzzy_lite {
#ifdef FL_USE_DOUBLE_PRECISION
    typedef double flScalar;
#else
    typedef float flScalar;
#endif
}

#endif	/* _FLSCALAR_H */

