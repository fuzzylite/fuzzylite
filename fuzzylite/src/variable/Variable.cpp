/*
 * Variable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "fl/variable/Variable.h"

#include "fl/term/Term.h"

#include <sstream>

namespace fl {

    Variable::Variable(const std::string& name)
            : _name(name) {
    }

    Variable::~Variable() {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            delete _terms[i];
        }
    }

    void Variable::configure(Configuration* config) {
        //do nothing
    }

    void Variable::setName(const std::string& name) {
        this->_name = name;
    }

    std::string Variable::getName() const {
        return this->_name;
    }

//It is assumed the terms are inserted in ascending order
    scalar Variable::minimum() const {
        if (_terms.size() > 0) return _terms[0]->minimum();
        return -std::numeric_limits<scalar>::infinity();
    }

    scalar Variable::maximum() const {
        if (_terms.size() > 0) return _terms[_terms.size() - 1]->maximum();
        return std::numeric_limits<scalar>::infinity();
    }

    std::string Variable::fuzzify(scalar x) const {
        std::ostringstream ss;
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << _terms[i]->membership(x) << "/" << _terms[i]->getName();
            if (i < _terms.size() - 1)
                ss << ", ";
        }
        return ss.str();
    }

    std::string Variable::toString() const {
        std::ostringstream ss;
        ss << getName() << " [";
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            ss << _terms[i]->toString();
            if (i < _terms.size() - 1) ss << ", ";
        }
        ss << "]";
        return ss.str();
    }

    /**
     * Operations for datatype _terms
     */
    void Variable::addTerm(Term* term) {
        this->_terms.push_back(term);
    }
    void Variable::insertTerm(Term* term, int index) {
        this->_terms.insert(this->_terms.begin() + index, term);
    }
    Term* Variable::getTerm(int index) const {
        return this->_terms[index];
    }
    Term* Variable::getTerm(const std::string& name) const {
        for (std::size_t i = 0; i < _terms.size(); ++i) {
            if (_terms[i]->getName() == name) {
                return _terms[i];
            }
        }
        return NULL;
    }

    bool Variable::hasTerm(const std::string& name) const {
        return getTerm(name) != NULL;
    }

    Term* Variable::removeTerm(int index) {
        Term* result = this->_terms[index];
        this->_terms.erase(this->_terms.begin() + index);
        return result;
    }
    int Variable::numberOfTerms() const {
        return this->_terms.size();
    }
    const std::vector<Term*>& Variable::terms() const {
        return this->_terms;
    }

}

