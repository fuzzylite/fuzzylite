/*
 * Variable.h
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#ifndef FL_VARIABLE_H_
#define FL_VARIABLE_H_

#include "../scalar.h"

#include <string>
#include <vector>
namespace fl {
class Term;
class Variable {
	protected:
		std::string _name;
		//TODO: change to unordered_map for C++11
		std::vector<Term*> _terms;

	public:
		Variable(const std::string& name = "");
		virtual ~Variable();

		virtual void setName(const std::string& name);
		virtual std::string getName() const;

		virtual void addTerm(Term* term);
		virtual Term* getTerm(const std::string& term) const;
		virtual Term* removeTerm(const std::string& term);
		virtual Term* getTerm(int index) const;
		virtual Term* removeTerm(int index);
		virtual int numberOfTerms() const;
		virtual std::vector<Term*> terms() const;

		virtual scalar minimum() const;
		virtual scalar maximum() const;

		virtual std::string fuzzify(scalar x) const;

		virtual std::string toString() const;

};

}

#endif /* FL_VARIABLE_H_ */
