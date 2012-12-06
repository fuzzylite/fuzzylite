/*
 * InfixToPostfix.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "Infix.h"

#include "Rule.h"

#include "../definitions.h"

#include <cmath>

#include <queue>
#include <stack>

namespace fl {

    Infix::Infix() {
        loadGenericFunctions();
        loadGenericOperators();
    }

    Infix::~Infix() {
        std::map<std::string, Infix::GenericOperator*>::const_iterator itGO = _genericOperators.begin();
        for (; itGO != _genericOperators.end(); ++itGO) {
            delete itGO->second;
        }

        std::map<std::string, Infix::GenericFunction*>::const_iterator itFO = _genericFunctions.begin();
        for (; itFO != _genericFunctions.end(); ++itFO) {
            delete itFO->second;
        }
    }

    bool Infix::isOperand(const std::string& name) const {
        //An operand is not a parenthesis...
        if (name == "(" || name == ")" || name == ",") return false;
        //nor an operator...
        if (isOperator(name)) return false;
        //nor a function...
        if (isFunction(name)) return false;
        //...it is everything else :)
        return true;
    }

    bool Infix::isOperator(const std::string& name) const {
        return _genericOperators.find(name) != _genericOperators.end();
    }

    bool Infix::isFunction(const std::string& name) const {
        return _genericFunctions.find(name) != _genericFunctions.end();
    }

    std::string Infix::toPostfix(const std::string& infixString) {
        //TODO: inserts spaces in all operators, functions, parentheses, and commas.
        std::stringstream ssInfix;
        for (std::size_t i = 0; i < infixString.size(); ++i) {
            if (infixString[i] == '(')
                ssInfix << " ( ";
            else if (infixString[i] == ')')
                ssInfix << " ) ";
            else if (infixString[i] == ',')
                ssInfix << " , ";
            else
                ssInfix << infixString[i];
        }
        std::string infix = ssInfix.str();
        FL_LOG("infix = " << infix);

        std::queue<std::string> queue;
        std::stack<std::string> stack;

        std::stringstream tokenizer(infix);
        std::string token;
        while (tokenizer >> token) {
            if (isOperand(token)) {
                queue.push(token);

            } else if (isFunction(token)) {
                stack.push(token);

            } else if (token == ",") {
                while (not stack.empty() and stack.top() != "(") {
                    queue.push(stack.top());
                    stack.pop();
                }
                if (stack.empty() or stack.top() != "(") {
                    FL_LOG("mismatching parentheses in: " << infixString);
                    throw std::exception();
                }

            } else if (isOperator(token)) {
                GenericOperator* op1 = getGenericOperator(token);
                while (true) {
                    GenericOperator* op2 = NULL;
                    if (not stack.empty() and isOperator(stack.top())) {
                        op2 = getGenericOperator(stack.top());
                    } else
                        break;

                    if ((op1->associativity < 0 and op1->precedence <= op2->precedence)
                            or op1->precedence < op2->precedence) {
                        queue.push(stack.top());
                        stack.pop();
                    } else
                        break;
                }
                stack.push(token);

            } else if (token == "(") {
                stack.push(token);

            } else if (token == ")") {
                while (not stack.empty() and stack.top() != "(") {
                    queue.push(stack.top());
                    stack.pop();
                }
                if (stack.empty() or stack.top() != "(") {
                    FL_LOG("mismatching parentheses in: " << infixString);
                }
                stack.pop(); //get rid of "("

                if (not stack.empty() and isFunction(stack.top())) {
                    queue.push(stack.top());
                    stack.pop();
                }
            } else {
                FL_LOG("this should have never occurred!");
                throw std::exception();
            }
        }

        while (not stack.empty()) {
            if (stack.top() == "(" or stack.top() == ")") {
                FL_LOG("mismatching parentheses in: " << infixString);
                throw std::exception();
            }
            queue.push(stack.top());
            stack.pop();
        }

        std::stringstream ssPostfix;
        while (not queue.empty()) {
            ssPostfix << queue.front();
            queue.pop();
            if (not queue.empty()) ssPostfix << " ";
        }
        return ssPostfix.str();
    }

    void Infix::loadGenericOperators() {
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

    void Infix::loadGenericFunctions() {
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
    void Infix::addGenericOperator(Infix::GenericOperator* genericOperator) {
        this->_genericOperators[genericOperator->name] = genericOperator;
    }
    Infix::GenericOperator* Infix::removeGenericOperator(const std::string& key) {
        std::map<std::string, GenericOperator*>::iterator it = this->_genericOperators.find(key);
        if (it == this->_genericOperators.end()) return NULL;
        GenericOperator* result = it->second;
        this->_genericOperators.erase(it);
        return result;
    }
    Infix::GenericOperator* Infix::getGenericOperator(const std::string& key) const {
        std::map<std::string, Infix::GenericOperator*>::const_iterator it = this->_genericOperators.find(key);
        if (it == this->_genericOperators.end()) return NULL;
        return it->second;
    }
    const std::map<std::string, Infix::GenericOperator*>& Infix::genericOperators() const {
        return this->_genericOperators;
    }

    /**
     * Operations for std::map _genericFunctions
     */
    void Infix::addGenericFunction(Infix::GenericFunction* genericFunction) {
        this->_genericFunctions[genericFunction->name] = genericFunction;
    }
    Infix::GenericFunction* Infix::removeGenericFunction(const std::string& key) {
        std::map<std::string, Infix::GenericFunction*>::iterator it = this->_genericFunctions.find(key);
        if (it == this->_genericFunctions.end()) return NULL;
        Infix::GenericFunction* result = it->second;
        this->_genericFunctions.erase(it);
        return result;
    }
    Infix::GenericFunction* Infix::getGenericFunction(const std::string& key) const {
        std::map<std::string, Infix::GenericFunction*>::const_iterator it = this->_genericFunctions.find(key);
        if (it == this->_genericFunctions.end()) return NULL;
        return it->second;
    }
    const std::map<std::string, Infix::GenericFunction*>& Infix::genericFunctions() const {
        return this->_genericFunctions;
    }

    void Infix::main() {
//        Infix::GenericFunction* f = ip.getGenericFunction("acos");
//        FL_LOG("acos(0.4) = " << f->oneArgFunction(0.4));
//        std::map<std::string, Infix::GenericFunction*> gf = ip.genericFunctions();
//        std::map<std::string, Infix::GenericFunction*>::const_iterator it = gf.begin();
//        for (; it != gf.end(); ++it) {
//            FL_LOG(it->first << "{" << it->second->toString() << "}");
//        }
        Infix infix;

        std::string notation = "3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3";
        FL_LOG(infix.toPostfix(notation));
        //3 4 2 * 1 5 - 2 3 ^ ^ / +


        notation = "3 + 4 * 2 / (1 - 5) ^ 2 ^ 3";
        FL_LOG(infix.toPostfix(notation));
        //3 4 2 * 1 5 - 2 3 ^ ^ / +

        notation = "sin(y * x) ^ 2 / x";
        FL_LOG(infix.toPostfix(notation));
        //y x * sin 2 ^ x /

        notation = "(Temperature is High and Oxigen is Low) or \
            (Temperature is Low and (Oxigen is Low or Oxigen is High))";
        FL_LOG(infix.toPostfix(notation));
        //Temperature is High Oxigen is Low and Temperature is Low Oxigen is Low Oxigen is High or and or


    }

} /* namespace fl */
