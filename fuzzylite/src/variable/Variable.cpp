/*
 * Variable.cpp
 *
 *  Created on: 2/12/2012
 *      Author: jcrada
 */

#include "Variable.h"

#include "../term/Term.h"

#include <sstream>

namespace fl {

Variable::Variable(const std::string& name)
		: _name(name) {
}

Variable::~Variable(){
	for (std::size_t  i= 0 ; i < _terms.size(); ++i){
		delete _terms[i];
	}
}

void Variable::setName(const std::string& name){
	this->_name = name;
}

std::string Variable::getName() const{
	return this->_name;
}

void Variable::addTerm(Term* term){
	this->_terms.push_back(term);
}

Term* Variable::getTerm(const std::string& name) const{
	for (std::size_t i = 0 ; i < _terms.size(); ++i){
		if (_terms[i]->getName() == name)
			return _terms[i];
	}
	return NULL;
}

Term* Variable::removeTerm(const std::string& name){
	//TODO
}

Term* Variable::getTerm(int index) const{
	return _terms[index];
}

Term* Variable::removeTerm(int index){
	//TODO
}

int Variable::numberOfTerms() const{
	return _terms.size();
}

std::vector<Term*> Variable::terms() const{
	return _terms;
}

//It is assumed the terms are inserted in ascending order
scalar Variable::minimum() const{
	return _terms[0]->minimum();
}

scalar Variable::maximum() const{
	return _terms[_terms.size() - 1]->maximum();
}

std::string Variable::fuzzify(scalar x) const{
	std::stringstream ss;
	for (std::size_t i = 0 ; i < _terms.size(); ++i){
		ss << _terms[i]->membership(x) << "/" << _terms[i]->getName();
		if (i < _terms.size() - 1) ss << ", ";
	}
	return ss.str();
}

std::string Variable::toString() const{
	return "Variable()";
}

}

