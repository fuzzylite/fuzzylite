/*
 * Exception.h
 *
 *  Created on: 14/12/2012
 *      Author: jcrada
 */

#ifndef FL_EXCEPTION_H
#define FL_EXCEPTION_H

#include "fl/config.h"
#include <exception>
#include <string>
#include <vector>

namespace fl {

    class FL_EXPORT Exception : public std::exception {
    protected:
        std::string _what;
    public:
        Exception(const std::string& what, const std::string& file, int line,
                const std::string& function, bool log = true);
        virtual ~Exception() throw ();

        virtual void setWhat(const std::string& what);
        virtual std::string getWhat() const;
        virtual const char* what() const throw ();
        
        virtual void append(const std::string& whatMore);
        virtual void append(const std::string& file, int line, const std::string& function);
        virtual void append(const std::string& whatMore,
            const std::string& file, int line, const std::string& function);

        static std::string btCallStack(const int maxCalls = 30);

        static void signalHandler(int signal);
    };

}
#endif /* FL_EXCEPTION_H */
