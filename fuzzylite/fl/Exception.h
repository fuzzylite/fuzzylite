/*
 * Exception.h
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXCEPTION_H_
#define FL_EXCEPTION_H_

#include "fl/definitions.h"
#include <exception>
#include <string>

namespace fl {

    class Exception : public std::exception {
    protected:
        std::string _what;
    public:
        Exception(const std::string& what, const std::string& file, int line,
                const std::string& function, bool log = true);
        virtual ~Exception() throw ();

        virtual void setWhat(const std::string& what);
        virtual void appendDetail(const std::string& detail);

        virtual const char* what() const throw ();
    };

}
#endif /* FL_EXCEPTION_H_ */
