/*
 * InfixToPostfix.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "InfixToPostfix.h"

#include "Rule.h"

#include <cmath>

namespace fl {

    InfixToPostfix::InfixToPostfix() {
        loadGenericFunctions();
        loadGenericOperators();
    }

    InfixToPostfix::~InfixToPostfix() {
        std::map<std::string, InfixToPostfix::GenericOperator*>::const_iterator itGO = _genericOperators.begin();
        for (; itGO != _genericOperators.end(); ++itGO) {
            delete itGO->second;
        }

        std::map<std::string, InfixToPostfix::GenericFunction*>::const_iterator itFO = _genericFunctions.begin();
        for (; itFO != _genericFunctions.end(); ++itFO) {
            delete itFO->second;
        }
    }

    void InfixToPostfix::loadGenericOperators() {
        char p = 7;
        // (!) Logical and (~) Bitwise NOT
        this->_genericOperators["!"] = new GenericOperator("!", p, 1);
        this->_genericOperators["~"] = new GenericOperator("~", p, 1);
        --p; //Power
        this->_genericOperators["^"] = new GenericOperator("^", p, 2, 1);
        --p; //Multiplication, Division, and Modulo
        this->_genericOperators["*"] = new GenericOperator("*", p);
        this->_genericOperators["/"] = new GenericOperator("/", p);
        this->_genericOperators["%"] = new GenericOperator("%", p);
        --p; //Addition, Subtraction
        this->_genericOperators["+"] = new GenericOperator("+", p);
        this->_genericOperators["-"] = new GenericOperator("-", p);
        --p; //Bitwise AND
        this->_genericOperators["&"] = new GenericOperator("&", p);
        --p; //Bitwise OR
        this->_genericOperators["|"] = new GenericOperator("|", p);
        --p; //Logical AND (symbolic and verbose)
        this->_genericOperators["&&"] = new GenericOperator("&&", p);
        this->_genericOperators[Rule::FL_AND] = new GenericOperator(Rule::FL_AND, p);
        this->_genericOperators[Rule::FL_AND]->mask = " " + Rule::FL_AND + " ";
        --p; //Logical OR (symbolic and verbose)
        this->_genericOperators["||"] = new GenericOperator("||", p);
        this->_genericOperators[Rule::FL_OR] = new GenericOperator(Rule::FL_OR, p);
        this->_genericOperators[Rule::FL_OR]->mask = " " + Rule::FL_OR + " ";
    }

    void InfixToPostfix::loadGenericFunctions() {
        this->_genericFunctions["acos"] = new GenericFunction("acos", &(std::acos));
        this->_genericFunctions["acosh"] = new GenericFunction("acosh", &(acosh));
        this->_genericFunctions["asin"] = new GenericFunction("asin", &(std::asin));
        this->_genericFunctions["asinh"] = new GenericFunction("asinh", &(asinh));
        this->_genericFunctions["atan"] = new GenericFunction("atan", &(std::atan));
        this->_genericFunctions["atan2"] = new GenericFunction("atan2", &(std::atan2));
        this->_genericFunctions["atanh"] = new GenericFunction("atanh", &(atanh));
        this->_genericFunctions["ceil"] = new GenericFunction("ceil", &(std::ceil));
        this->_genericFunctions["cos"] = new GenericFunction("cos", &(std::cos));
        this->_genericFunctions["cosh"] = new GenericFunction("cosh", &(std::cosh));
        this->_genericFunctions["exp"] = new GenericFunction("exp", &(std::exp));
        this->_genericFunctions["fabs"] = new GenericFunction("fabs", &(std::fabs));
        this->_genericFunctions["floor"] = new GenericFunction("floor", &(std::floor));
        this->_genericFunctions["log"] = new GenericFunction("log", &(std::log));
        this->_genericFunctions["log10"] = new GenericFunction("log10", &(std::log10));
        this->_genericFunctions["log1p"] = new GenericFunction("log1p", &(log1p));
        this->_genericFunctions["pow"] = new GenericFunction("pow", &(std::pow));
        this->_genericFunctions["sin"] = new GenericFunction("sin", &(std::sin));
        this->_genericFunctions["sinh"] = new GenericFunction("sinh", &(std::sinh));
        this->_genericFunctions["sqrt"] = new GenericFunction("sqrt", &(std::sqrt));
        this->_genericFunctions["tan"] = new GenericFunction("tan", &(std::tan));
        this->_genericFunctions["tanh"] = new GenericFunction("tanh", &(std::tanh));
//        this->_genericFunctions["X"] = new GenericFunction("X", &(std::X));
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
