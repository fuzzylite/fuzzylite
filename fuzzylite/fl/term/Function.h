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
            virtual ~Element();

            virtual std::string toString() const = 0;

        };

        typedef double(*Unary)(double);
        typedef double(*Binary)(double, double);

        struct FL_EXPORT Operator : public Element {
            Unary unary;
            Binary binary;
            short precedence;
            short arity;
            short associativity;
            Operator(const std::string& name, Unary unary, short precedence = 0,
                    short associativity = -1);
            Operator(const std::string& name, Binary unary, short precedence = 0,
                    short associativity = -1);

            std::string toString() const;
        };

        struct FL_EXPORT BuiltInFunction : public Element {
            Unary unary;
            Binary binary;
            short arity;
            short associativity;

            BuiltInFunction(const std::string& name, Unary functionPointer,
                    short associativity = -1);
            BuiltInFunction(const std::string& name, Binary functionPointer,
                    short associativity = -1);
            std::string toString() const;
        };

        /**************************
         * Tree elements, wrap Elements into Nodes.
         **************************/

        struct FL_EXPORT Node {
            Operator* foperator;
            BuiltInFunction* function;
            std::string reference;
            scalar value;
            Node* left;
            Node* right;

            Node(Operator* foperator, Node* left = NULL, Node* right = NULL);
            Node(BuiltInFunction* function, Node* left = NULL, Node* right = NULL);
            Node(const std::string& reference);
            Node(scalar value);

            scalar evaluate(const std::map<std::string, scalar>*
                    references = NULL) const;

            std::string toString() const;
            std::string toPrefix(const Node* node = NULL) const;
            std::string toInfix(const Node* node = NULL) const;
            std::string toPostfix(const Node* node = NULL) const;
        };




        /******************************
         * Term
         ******************************/
    protected:
        std::string _infix;
        const Engine* _engine;
        Node* _root;

        virtual void loadOperators();
        virtual void loadBuiltInFunctions();

        /**
         * Parsing methods
         */

        Operator* getOperator(const std::string& key) const;

        BuiltInFunction* getBuiltInFunction(const std::string& key) const;


    public:
        std::map<std::string, scalar> variables;
        std::map<std::string, Operator*> operators;
        std::map<std::string, BuiltInFunction*> functions;
        Function(const std::string& name = "",
                const std::string& infix = "", const Engine* engine = NULL,
                bool loadBuiltInFunctions = true);
        virtual ~Function();

        static Function* create(const std::string& name,
                const std::string& infix,
                const Engine* engine = NULL) throw (fl::Exception);

        virtual scalar membership(scalar x) const;

        virtual std::string className() const;
        virtual std::string toString() const;

        virtual Function* copy() const;

        virtual void setInfix(const std::string& infix);
        virtual std::string getInfix() const;
        
        virtual void setEngine(const Engine* engine);
        virtual const Engine* getEngine() const;
        
        virtual void load() throw (fl::Exception);

        virtual void load(const std::string& infix,
                const Engine* engine = NULL) throw (fl::Exception);

        virtual Node* parse(const std::string& infix) throw (fl::Exception);

        virtual std::string toPostfix(const std::string& infix) const throw (fl::Exception);

        virtual std::string space(const std::string& infix) const;

        virtual bool isOperand(const std::string& token) const;
        virtual bool isBuiltInFunction(const std::string& token) const;
        virtual bool isOperator(const std::string& token) const;


        //        static void doSomething(scalar function(scalar, scalar));

        static void main();

    };

}

#endif	/* FL_FUNCTION_H */

