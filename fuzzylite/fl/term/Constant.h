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
        Constant(const std::string& name = "", 
                scalar value = fl::nan);
        virtual ~Constant();

        virtual scalar membership(scalar x) const;

        virtual std::string className() const;
        virtual std::string toString() const;

        virtual Constant* copy() const;

        virtual void setValue(scalar value);
        virtual scalar getValue() const;

    };
}

#endif	/* FL_CONSTANT_H */

