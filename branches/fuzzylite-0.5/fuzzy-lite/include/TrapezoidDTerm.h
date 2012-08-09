/* 
 * File:   TrapezoidTerm.h
 * Author: jcrada
 *
 * Created on November 1, 2009, 1:48 PM
 */

#ifndef _TRAPEZOIDDTERM_H
#define	_TRAPEZOIDDTERM_H

#include "defs.h"
#include "DiscreteTerm.h"

namespace fuzzy_lite {

    class TrapezoidDTerm : public DiscreteTerm {
    public:
        TrapezoidDTerm(flScalar a, flScalar b, flScalar c, flScalar d);
        TrapezoidDTerm(const std::string& name, flScalar a, flScalar b, flScalar c, flScalar d);
        TrapezoidDTerm(const std::string& name, flScalar a, flScalar d, flScalar simmetry = 0.25); //simmetry= 0 -> square, 0.5->triangle
        virtual ~TrapezoidDTerm();
    };
}
#endif	/* _TRAPEZOIDDTERM_H */

