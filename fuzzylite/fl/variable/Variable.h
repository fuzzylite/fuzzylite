/*
 * Variable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_VARIABLE_H_
#define FL_VARIABLE_H_

#include "fl/scalar.h"

#include <string>
#include <vector>
namespace fl {

    class Term;
    class Configuration;

    class Variable {
    protected:
        std::string _name;
        //TODO: change to unordered_map for C++11
        std::vector<Term*> _terms;

    public:
        Variable(const std::string& name = "");
        Variable(const Variable& copy);

        virtual ~Variable();

        virtual void configure(Configuration* config);

        virtual void setName(const std::string& name);
        virtual std::string getName() const;

        virtual scalar minimum() const;
        virtual scalar maximum() const;

        virtual std::string fuzzify(scalar x) const;

        virtual std::string toString() const;

        /**
         * Operations for iterable datatype _terms
         */
        virtual void addTerm(Term* term);
        virtual void insertTerm(Term* term, int index);
        virtual Term* getTerm(int index) const;
        virtual Term* getTerm(const std::string& name) const;
        virtual bool hasTerm(const std::string& name) const;
        virtual Term* removeTerm(int index);
        virtual int numberOfTerms() const;
        virtual const std::vector<Term*>& terms() const;

    };

}

#endif /* FL_VARIABLE_H_ */
