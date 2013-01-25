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

    class Exception : public std::exception {
    protected:
        std::string _what;
        std::vector<std::string> _details;
        std::vector<std::string> _calls;
    public:
        Exception(const std::string& what, const std::string& file, int line,
                const std::string& function, bool log = true);
        virtual ~Exception() throw ();

        virtual void setWhat(const std::string& what);
        virtual const char* what() const throw ();
        
        static std::string btCallStack();
        
        static void signalHandler(int signal);
        
        
        

        /**
         * Operations for std::vector _details
         */
        virtual void addDetail(const std::string& detail);
        virtual void addDetail(const std::string& detail,
                const std::string& file, int line, const std::string& function);
        virtual std::string getDetail(int index) const;
        virtual std::string removeDetail(int index);
        virtual int numberOfDetails() const;
        virtual const std::vector<std::string>& details() const;

        /**
         * Operations for std::vector _calls
         */
        virtual void addCall(const std::string& file, int line, const std::string& function);
        virtual std::string getCall(int index) const;
        virtual std::string removeCall(int index);
        virtual int numberOfCalls() const;
        virtual const std::vector<std::string>& calls() const;


    };

}
#endif /* FL_EXCEPTION_H */
