/*
 * Term.h
 *
 *  Created on: 29/11/2012
 *      Author: jcrada
 */

#ifndef FL_TERM_H_
#define FL_TERM_H_

#include "fl/scalar.h"

#include <string>
#include <limits>

namespace fl {

    class Term {
    protected:
        std::string _name;

    public:

        Term(const std::string& name = "")
        : _name(name) { }

        virtual ~Term() { }

        virtual void setName(const std::string& name) {
            this->_name = name;
        }

        virtual std::string getName() const {
            return this->_name;
        }

        virtual scalar membership(scalar x) const = 0;

        virtual std::string className() const = 0;
        virtual std::string toString() const = 0;

        virtual Term* copy() const = 0;


    };

} 
#endif /* FL_TERM_H_ */
