/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * InfixToPostfix.cpp
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#include "fl/rule/Infix.h"

#include "fl/rule/Rule.h"
#include "fl/operator/Operator.h"



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

        std::map<std::string, Infix::GenericFunction*>::const_iterator itGF = _genericFunctions.begin();
        for (; itGF != _genericFunctions.end(); ++itGF) {
            delete itGF->second;
        }
    }

    bool Infix::isOperand(const std::string& name) const {
        //An operand is not a parenthesis...
        if (name == "(" or name == ")" or name == ",") return false;
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
        //TODO: inserts spaces in all operators, parentheses, and commas.
//        std::vector<std::string> space;
//        std::map<std::string, GenericOperator*>::const_iterator itGO = this->_genericOperators.begin();
//        for (; itGO != this->_genericOperators.end(); ++itGO) {
//            if (itGO->first == Rule::FL_AND or itGO->first == Rule::FL_OR)
//                continue;
//            space.push_back(itGO->first);
//        }
//        space.push_back("(");
//        space.push_back(")");
//        space.push_back(",");
//
        std::string infix = infixString;
//        for (std::size_t i = 0; i < space.size(); ++i) {
//            infix = Op::findReplace(infix, space[i], " " + space[i] + " ", true);
//        }
////        FL_LOG("infix=" << infix);
        
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
                    std::ostringstream ex;
                    ex << "mismatching parentheses in: " << infixString;
                    throw fl::Exception(ex.str(), FL_AT);
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
                    std::ostringstream ex;
                    ex << "mismatching parentheses in: " << infixString;
                    throw fl::Exception(ex.str(), FL_AT);
                }
                stack.pop(); //get rid of "("

                if (not stack.empty() and isFunction(stack.top())) {
                    queue.push(stack.top());
                    stack.pop();
                }
            } else {
                std::ostringstream ex;
                ex << "this should have never occurred!";
                throw fl::Exception(ex.str(), FL_AT);
            }
        }

        while (not stack.empty()) {
            if (stack.top() == "(" or stack.top() == ")") {
                std::ostringstream ex;
                ex << "mismatching parentheses in: " << infixString;
                throw fl::Exception(ex.str(), FL_AT);
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
        this->_genericOperators[Rule::FL_AND] = new GenericOperator(Rule::FL_AND, p);
        --p; //Logical OR (symbolic and verbose)
        this->_genericOperators[Rule::FL_OR] = new GenericOperator(Rule::FL_OR, p);

        //Excluded to provide simplicity due to spacing all these characters before & |
        //TODO: Incorporate them as long as they are spaced properly before parsing.
        //        this->_genericOperators["&&"] = new GenericOperator("&&", p);
        //        this->_genericOperators["&&"]->regex = "\\&\\&";
        //        this->_genericOperators["||"] = new GenericOperator("||", p);
        //        this->_genericOperators["||"]->regex = "\\|\\|";
    }

    void Infix::loadGenericFunctions() {
        this->_genericFunctions["acos"] = new GenericFunction("acos", &(std::acos));
        this->_genericFunctions["asin"] = new GenericFunction("asin", &(std::asin));
        this->_genericFunctions["atan"] = new GenericFunction("atan", &(std::atan));
        this->_genericFunctions["atan2"] = new GenericFunction("atan2", &(std::atan2));
        this->_genericFunctions["ceil"] = new GenericFunction("ceil", &(std::ceil));
        this->_genericFunctions["cos"] = new GenericFunction("cos", &(std::cos));
        this->_genericFunctions["cosh"] = new GenericFunction("cosh", &(std::cosh));
        this->_genericFunctions["exp"] = new GenericFunction("exp", &(std::exp));
        this->_genericFunctions["fabs"] = new GenericFunction("fabs", &(std::fabs));
        this->_genericFunctions["floor"] = new GenericFunction("floor", &(std::floor));
        this->_genericFunctions["log"] = new GenericFunction("log", &(std::log));
        this->_genericFunctions["log10"] = new GenericFunction("log10", &(std::log10));
        this->_genericFunctions["pow"] = new GenericFunction("pow", &(std::pow));
        this->_genericFunctions["sin"] = new GenericFunction("sin", &(std::sin));
        this->_genericFunctions["sinh"] = new GenericFunction("sinh", &(std::sinh));
        this->_genericFunctions["sqrt"] = new GenericFunction("sqrt", &(std::sqrt));
        this->_genericFunctions["tan"] = new GenericFunction("tan", &(std::tan));
        this->_genericFunctions["tanh"] = new GenericFunction("tanh", &(std::tanh));
		
		#ifdef FL_UNIX
		//not found in Windows
		this->_genericFunctions["log1p"] = new GenericFunction("log1p", &(log1p));
		this->_genericFunctions["acosh"] = new GenericFunction("acosh", &(acosh));
        this->_genericFunctions["asinh"] = new GenericFunction("asinh", &(asinh));
        this->_genericFunctions["atanh"] = new GenericFunction("atanh", &(atanh));
		#endif
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
        Infix infix;
//        GenericFunction* f = infix.getGenericFunction("acos");
//        FL_LOG("acos(0.4) = " << f->oneArgFunction(0.4));
        std::map<std::string, GenericFunction*> gf = infix.genericFunctions();
        std::map<std::string, GenericFunction*>::const_iterator it = gf.begin();
        for (; it != gf.end(); ++it) {
            FL_LOG(it->first << "{" << it->second->toString() << "}");
        }

        std::string notation = "3+4*2/(1-5)^2^3";
        FL_LOG(infix.toPostfix(notation));
        //3 4 2 * 1 5 - 2 3 ^ ^ / +

        notation = "3+4*2/(1-5)^2^3";
        FL_LOG(infix.toPostfix(notation));
        //3 4 2 * 1 5 - 2 3 ^ ^ / +

        notation = "sin(y*x)^2/x";
        FL_LOG(infix.toPostfix(notation));
        //y x * sin 2 ^ x /

        notation = "(Temperature is High and Oxigen is Low) or \
            (Temperature is Low and (Oxigen is Low or Oxigen is High))";
        FL_LOG(infix.toPostfix(notation));
        //Temperature is High Oxigen is Low and Temperature is Low Oxigen is Low Oxigen is High or and or

    }

} 
