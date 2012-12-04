/*
 * InfixToPostfix.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "InfixToPostfix.h"

namespace fl {

    InfixToPostfix::InfixToPostfix() {
    }

    InfixToPostfix::~InfixToPostfix() {
        std::map<std::string, InfixToPostfix::GenericOperator>::const_iterator itGO = _genericOperators.begin();
        for (; itGO != _genericOperators.end(); ++itGO){
            delete itGO->second;
        }

        std::map<std::string, InfixToPostfix::GenericFunction>::const_iterator itFO = _genericFunctions.begin();
        for (; itFO != _genericFunctions.end(); ++itFO){
            delete itFO->second;
        }
    }

    void InfixToPostfix::loadGenericOperators() {
    }
    void InfixToPostfix::loadGenericFunctions() {
    }

    /**
     * Operations for std::map _genericOperators
     */
    void InfixToPostfix::addGenericOperator(InfixToPostfix::GenericOperator* genericOperator) {
        this->_genericOperators[genericOperator->name] = genericOperator;
    }
    InfixToPostfix::GenericOperator* InfixToPostfix::removeGenericOperator(const std::string& key) {
        std::map<std::string, GenericOperator*>::iterator it = this->_genericOperators.find(key);
        if (it == this->_genericOperators.end()) return NULL;
        GenericOperator* result = it->second;
        this->_genericOperators.erase(it);
        return result;
    }
    InfixToPostfix::GenericOperator* InfixToPostfix::getGenericOperator(const std::string& key) const {
        std::map<std::string, InfixToPostfix::GenericOperator*>::const_iterator it = this->_genericOperators.find(key);
        if (it == this->_genericOperators.end()) return NULL;
        return it->second;
    }
    const std::map<std::string, InfixToPostfix::GenericOperator*>& InfixToPostfix::genericOperators() const {
        return this->_genericOperators;
    }

    /**
     * Operations for std::map _genericFunctions
     */
    void InfixToPostfix::addGenericFunction(InfixToPostfix::GenericFunction* genericFunction) {
        this->_genericFunctions[genericFunction->name] = genericFunction;
    }
    InfixToPostfix::GenericFunction* InfixToPostfix::removeGenericFunction(const std::string& key) {
        std::map<std::string, InfixToPostfix::GenericFunction*>::iterator it = this->_genericFunctions.find(key);
        if (it == this->_genericFunctions.end()) return NULL;
        InfixToPostfix::GenericFunction* result = it->second;
        this->_genericFunctions.erase(it);
        return result;
    }
    InfixToPostfix::GenericFunction* InfixToPostfix::getGenericFunction(const std::string& key) const {
        std::map<std::string, InfixToPostfix::GenericFunction*>::const_iterator it = this->_genericFunctions.find(key);
        if (it == this->_genericFunctions.end()) return NULL;
        return it->second;
    }
    const std::map<std::string, InfixToPostfix::GenericFunction*>& InfixToPostfix::genericFunctions() const {
        return this->_genericFunctions;
    }

} /* namespace fl */
