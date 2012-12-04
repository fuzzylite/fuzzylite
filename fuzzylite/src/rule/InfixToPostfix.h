/*
 * InfixToPostfix.h
 *
 *  Created on: 4/12/2012
 *      Author: jcrada
 */

#ifndef FL_INFIXTOPOSTFIX_H_
#define FL_INFIXTOPOSTFIX_H_

#include <map>
#include <string>

namespace fl {

    class InfixToPostfix {
    public:

        class GenericOperator {
        public:
            std::string name;
            std::string mask;
            char precedence;
            char arity;
            char associativity;
        };

//        typedef scalar (*OneArgFunction)(scalar);
//        typedef scalar (*TwoArgFunction)(scalar, scalar);

        class GenericFunction {
        public:
            std::string name;
            char arity;
            char associativity;
//            OneArgFunction oneArgFunction;
//            TwoArgFunction twoArgFunction;
        };

    protected:
        std::map<std::string, GenericOperator*> _genericOperators;
        std::map<std::string, GenericFunction*> _genericFunctions;

        void loadGenericOperators();
        void loadGenericFunctions();

    public:
        InfixToPostfix();
        ~InfixToPostfix();

        /**
         * Operations for std::map _genericOperators
         */
         void addGenericOperator(GenericOperator* genericOperator);
         GenericOperator* removeGenericOperator(const std::string& key);
         GenericOperator* getGenericOperator(const std::string& key) const;
         const std::map<std::string, GenericOperator*>& genericOperators() const;

        /**
         * Operations for std::map _genericFunctions
         */
         void addGenericFunction(GenericFunction* genericFunction);
         GenericFunction* removeGenericFunction(const std::string& key);
         GenericFunction* getGenericFunction(const std::string& key) const;
         const std::map<std::string, GenericFunction*>& genericFunctions() const;

        std::string convert(const std::string& infix);

    };

} /* namespace fl */
#endif /* FL_INFIXTOPOSTFIX_H_ */
