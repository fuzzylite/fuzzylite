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
 * Infix.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_INFIX_H
#define FL_INFIX_H

#include "fl/fuzzylite.h"



#include <map>
#include <string>
#include <sstream>
namespace fl {

    class FL_EXPORT Infix {
    public:

        class FL_EXPORT GenericOperator {
        public:
            std::string name;
            short precedence;
            short arity;
            short associativity;

            GenericOperator(const std::string& name, short precedence, short arity = 2,
                    short associativity = -1)
            : name(name), precedence(precedence), arity(arity), associativity(associativity) { }

            std::string toString() const {
                std::stringstream ss;
                ss << "name='" << name << "' "
                        << "precedence='" << precedence << "' "
                        << "arity='" << arity << "' "
                        << "associativity='" << associativity << "' "
                        ;
                return ss.str();
            }

        };

        typedef double(*OneArgFunction)(double);
        typedef double(*TwoArgFunction)(double, double);

        class FL_EXPORT GenericFunction {
        public:
            std::string name;
            short arity;
            short associativity;
            OneArgFunction oneArgFunction;
            TwoArgFunction twoArgFunction;

            GenericFunction(const std::string& name, short arity = 1)
            : name(name), arity(arity), associativity(-1),
            oneArgFunction(NULL), twoArgFunction(NULL) { }

            GenericFunction(const std::string&name, OneArgFunction oneArgFunction)
            : name(name), arity(1), associativity(-1), oneArgFunction(oneArgFunction),
            twoArgFunction(NULL) { }

            GenericFunction(const std::string&name, TwoArgFunction twoArgFunction)
            : name(name), arity(2), associativity(-1), oneArgFunction(NULL),
            twoArgFunction(twoArgFunction) { }

            std::string toString() const {
                std::stringstream ss;
                ss << "name='" << name << "' "
                        << "arity='" << arity << "' "
                        << "associativity='" << associativity << "' "
                        ;
                return ss.str();
            }
        };

    protected:
        std::map<std::string, GenericOperator*> _genericOperators;
        std::map<std::string, GenericFunction*> _genericFunctions;

        virtual void loadGenericOperators();
        virtual void loadGenericFunctions();

    public:
        Infix();
        virtual ~Infix();

        virtual std::string toPostfix(const std::string& infixString);

        virtual bool isOperand(const std::string& token) const;
        virtual bool isOperator(const std::string& token) const;
        virtual bool isFunction(const std::string& token) const;

        /**
         * Operations for std::map _genericOperators
         */
        virtual void addGenericOperator(GenericOperator* genericOperator);
        virtual GenericOperator* removeGenericOperator(const std::string& key);
        virtual GenericOperator* getGenericOperator(const std::string& key) const;
        virtual const std::map<std::string, GenericOperator*>& genericOperators() const;

        /**
         * Operations for std::map _genericFunctions
         */
        virtual void addGenericFunction(GenericFunction* genericFunction);
        virtual GenericFunction* removeGenericFunction(const std::string& key);
        virtual GenericFunction* getGenericFunction(const std::string& key) const;
        virtual const std::map<std::string, GenericFunction*>& genericFunctions() const;

        static void main();
    };

} 
#endif /* FL_INFIX_H */
