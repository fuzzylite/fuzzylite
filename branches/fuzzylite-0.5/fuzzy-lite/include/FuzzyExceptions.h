/* 
 * File:   FuzzyExceptions.h
 * Author: jcrada
 *
 * Created on November 13, 2009, 3:34 PM
 */

#ifndef _FUZZYEXCEPTIONS_H
#define	_FUZZYEXCEPTIONS_H

#include "FuzzyException.h"
#include <sstream>
namespace fuzzy_lite {

    class OutOfRangeException : public FuzzyException {
    public:
        OutOfRangeException(const std::string& file, int line, const std::string& function,
                const std::string& message);
        OutOfRangeException(const std::string& file, int line, const std::string& function,
                const std::string& message, const FuzzyException& previous_exception);
        virtual ~OutOfRangeException() throw ();

        virtual std::string getName() const throw ();

        static void CheckArray(const std::string& file, int line, const std::string& function,
                int index, int max); 
    };

    class InvalidArgumentException : public FuzzyException {
    public:
        InvalidArgumentException(const std::string& file, int line, const std::string& function,
                const std::string& message);
        InvalidArgumentException(const std::string& file, int line, const std::string& function,
                const std::string& message, const FuzzyException& previous_exception);
        virtual ~InvalidArgumentException() throw ();

        virtual std::string getName() const throw ();
    };

    class NullPointerException : public FuzzyException {
    public:
        NullPointerException(const std::string& file, int line, const std::string& function,
                const std::string& message);
        NullPointerException(const std::string& file, int line, const std::string& function,
                const std::string& message, const FuzzyException& previous_exception);
        virtual ~NullPointerException() throw ();

        virtual std::string getName() const throw ();
    };

    class RuleParsingException : public FuzzyException {
    public:
        RuleParsingException(const std::string& file, int line, const std::string& function,
                const std::string& message);
        RuleParsingException(const std::string& file, int line, const std::string& function,
                const std::string& message, const FuzzyException& previous_exception);
        virtual ~RuleParsingException() throw ();

        virtual std::string getName() const throw ();
    };

}

#endif	/* _FUZZYEXCEPTIONS_H */

