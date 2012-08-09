/* 
 * File:   TriangularTerm.h
 * Author: jcrada
 *
 * Created on October 30, 2009, 7:44 PM
 */

#ifndef _TRIANGULARDTERM_H
#define	_TRIANGULARDTERM_H

#include "DiscreteTerm.h"
namespace fuzzy_lite {

    class TriangularDTerm : public DiscreteTerm {
    public:
        TriangularDTerm(flScalar a, flScalar b, flScalar c);
        TriangularDTerm(const std::string& name, flScalar a, flScalar c);
        TriangularDTerm(const std::string& name, flScalar a, flScalar b, flScalar c);
        virtual ~TriangularDTerm();
    };
}
#endif	/* _TRIANGULARDTERM_H */

