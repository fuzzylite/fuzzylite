/* 
 * File:   Function.h
 * Author: jcrada
 *
 * Created on 30 March 2013, 1:13 PM
 */

#ifndef FL_FUNCTION_H
#define	FL_FUNCTION_H

#include "fl/term/Term.h"

#include <map>
#include <string>

namespace fl {

    class Engine;

    class FL_EXPORT Function : public Term {
    public:

        /****************************
         * Parsing Elements
         ****************************/

    public:

        struct FL_EXPORT Element {
            std::string name;
            Element(const std::string& name);

            virtual std::string toString() const = 0;

        };

        typedef double(*Unary)(double);
        typedef double(*Binary)(double, double);
        //        typedef double(*Tertiary)(double, double, double);

        template <typename T>
        struct FL_EXPORT Operator : public Element {
            T functionPointer;
            short precedence;
            short arity;
            short associativity;
            Operator(const std::string& name, T functionPointer, short precedence = 0,
                    short arity = 2, short associativity = -1);

            std::string toString() const;
        };

        template <typename T>
        struct FL_EXPORT BuiltInFunction : public Element {
            T functionPointer;
            short arity;
            short associativity;

            BuiltInFunction(const std::string& name, T functionPointer,
                    short arity = 1, short associativity = -1);
            std::string toString() const;
        };

        //        template <class T>
        //        struct FL_EXPORT LinguisticVariable<T> : public Element{
        //            T variable;
        //            LinguisticVariable(T variable);
        //            
        //            scalar evaluate() const;
        //            std::string toString() const;
        //        };

        /**************************
         * Tree elements
         **************************/

        /******************************
         * Term
         ******************************/
    protected:
        std::string _infix;
        const Engine* _engine;
        //        std::map<std::string, Operator<Unary>*> _unaryOperators;
        std::map<std::string, Operator<Binary>*> _binaryOperators;
        std::map<std::string, BuiltInFunction<Unary>*> _unaryFunctions;
        std::map<std::string, BuiltInFunction<Binary>*> _binaryFunctions;

        virtual void loadOperators();
        virtual void loadBuiltInFunctions();

        /**
         * Parsing methods
         */

        //        template <typename T>
        Operator<Binary>* getOperator(const std::string& key) const;

        template <typename T>
        BuiltInFunction<T>* getBuiltInFunction(const std::string& key) const;


    public:
        Function(const std::string& name = "", bool loadBuiltInFunctions = true);
        virtual ~Function();

        static Function* create(const std::string& name,
                const std::string& infix,
                const Engine* engine = NULL) throw (fl::Exception);

        virtual scalar membership(scalar x) const;

        virtual std::string className() const;
        virtual std::string toString() const;

        virtual Function* copy() const;

        virtual void load(const std::string& infix,
                const Engine* engine = NULL) throw (fl::Exception);
        virtual std::string getInfix() const;
        virtual const Engine* getEngine() const;

        virtual std::string toPostfix(const std::string& infix) const;
        virtual bool isOperand(const std::string& token) const;
        virtual bool isBuiltInFunction(const std::string& token) const;
        virtual bool isOperator(const std::string& token) const;
        
        
        //        static void doSomething(scalar function(scalar, scalar));

        static void main();

    };

}

#endif	/* FL_FUNCTION_H */

