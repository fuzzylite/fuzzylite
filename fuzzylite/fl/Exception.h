/*
 * Exception.h
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXCEPTION_H_
#define FL_EXCEPTION_H_

#include <exception>
#include <string>

namespace fl {

    class Exception : public std::exception{
    protected:
        std::string _what;
    public:
        Exception(const std::string& what, bool log = true);
        virtual ~Exception() throw ();

        virtual void setWhat(const std::string& what);

        virtual const char* what() const throw ();
    };

} /* namespace fl */
#endif /* FL_EXCEPTION_H_ */
