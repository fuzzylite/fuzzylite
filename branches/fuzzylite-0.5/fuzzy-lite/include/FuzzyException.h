/* 
 * File:   FuzzyException.h
 * Author: jcrada
 *
 * Created on November 11, 2009, 7:15 AM
 */

#ifndef _FUZZYEXCEPTION_H
#define	_FUZZYEXCEPTION_H

#include <string>
#include <exception>

namespace fuzzy_lite {

    class FuzzyException : public std::exception {
    private:
        std::string _message;
        std::string _location;
        const FuzzyException* _previous_exception;
    public:
        FuzzyException(const std::string& file, int line, const std::string& function);
        FuzzyException(const std::string& file, int line, const std::string& function,
                const std::string& message);
        FuzzyException(const std::string& file, int line, const std::string& function,
                const std::string& message, const FuzzyException& previous_exception);
        virtual ~FuzzyException() throw ();

        virtual std::string getName() const throw();

        virtual void setMessage(const std::string& message) throw();
        virtual std::string getMessage() const throw();

        virtual void setLocation(const std::string& file, int line, const std::string& function) throw();
        virtual std::string getLocation() const throw();

        virtual void setPreviousException(const FuzzyException& exception) throw();
        virtual const FuzzyException* getPreviousException() const throw();

        virtual const char* what() const throw ();
        virtual std::string toString() const throw();
        virtual std::string getStackTrace() const throw();

        static void main(int argc, char** argv);
    };

    
}


#endif	/* _FUZZYEXCEPTION_H */

