/* 
 * File:   Constant.h
 * Author: jcrada
 *
 * Created on 8 May 2013, 8:21 PM
 */

#ifndef FL_CONSTANT_H
#define	FL_CONSTANT_H

#include "fl/term/Term.h"

namespace fl {

    class FL_EXPORT Constant : public Term {
    protected:
        scalar _value;

    public:
        Constant(const std::string& name = "", scalar value);
        virtual ~Constant();

        virtual scalar membership(scalar x) const = 0;

        virtual std::string className() const = 0;
        virtual std::string toString() const = 0;

        virtual Term* copy() const = 0;

        virtual void setValue(scalar value);
        virtual scalar getValue() const;

    };
}

#endif	/* FL_CONSTANT_H */

