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
        typedef scalar(*Unary)(scalar);
        typedef scalar(*Binary)(scalar, scalar);

        struct FL_EXPORT Element {
            std::string name;
            Unary unary;
            Binary binary;
            int arity;
            int associativity;
            Element(const std::string& name);
            Element(const std::string& name, Unary unary, int associativity = -1);
            Element(const std::string& name, Binary binary, int associativity = -1);
            virtual ~Element();

            virtual std::string toString() const = 0;

        };

        struct FL_EXPORT Operator : public Element {
            int precedence;

            Operator(const std::string& name, Unary unary, int precedence = 0, int associativity = -1);
            Operator(const std::string& name, Binary unary, int precedence = 0, int associativity = -1);

            std::string toString() const;
        };

        struct FL_EXPORT BuiltInFunction : public Element {
            BuiltInFunction(const std::string& name, Unary functionPointer, int associativity = -1);
            BuiltInFunction(const std::string& name, Binary functionPointer, int associativity = -1);
            std::string toString() const;
        };

        /**************************
         * Tree elements, wrap Elements into Nodes.
         **************************/

        struct FL_EXPORT Node {
            Operator* foperator;
            BuiltInFunction* function;
            std::string variable;
            scalar value;
            Node* left;
            Node* right;

            Node(Operator* foperator, Node* left = NULL, Node* right = NULL);
            Node(BuiltInFunction* function, Node* left = NULL, Node* right = NULL);
            Node(const std::string& variable);
            Node(scalar value);

            scalar evaluate(const std::map<std::string, scalar>*
                    variables = NULL) const;

            std::string toString() const;
            std::string toPrefix(const Node* node = NULL) const;
            std::string toInfix(const Node* node = NULL) const;
            std::string toPostfix(const Node* node = NULL) const;
        };




        /******************************
         * Term
         ******************************/

    private:
        virtual void loadOperators();

    protected:
        std::string _formula;
        const Engine* _engine;

        /**
         * Parsing methods
         */

        Operator* getOperator(const std::string& key) const;

        BuiltInFunction* getBuiltInFunction(const std::string& key) const;


    public:
        Node* root;
        std::map<std::string, scalar> variables;
        std::map<std::string, Operator*> operators;
        std::map<std::string, BuiltInFunction*> functions;
        Function(const std::string& name = "",
                const std::string& formula = "", const Engine* engine = NULL);
        virtual ~Function();

        static Function* create(const std::string& name,
                const std::string& formula,
                const Engine* engine = NULL,
                bool requiresFunctions = true) throw (fl::Exception);

        virtual scalar membership(scalar x) const;

        virtual scalar evaluate(const std::map<std::string, scalar>* variables) const;

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual void setFormula(const std::string& formula);
        virtual std::string getFormula() const;

        virtual void setEngine(const Engine* engine);
        virtual const Engine* getEngine() const;

        virtual void load() throw (fl::Exception);

        virtual void load(const std::string& formula,
                const Engine* engine = NULL) throw (fl::Exception);

        virtual void loadBuiltInFunctions();

        virtual Node* parse(const std::string& formula) throw (fl::Exception);

        virtual std::string toPostfix(const std::string& formula) const throw (fl::Exception);

        virtual std::string space(const std::string& formula) const;

        virtual bool isOperand(const std::string& token) const;
        virtual bool isBuiltInFunction(const std::string& token) const;
        virtual bool isOperator(const std::string& token) const;

        virtual Function* copy() const;

        static Term* constructor();

        static void main();

    };

}

#endif	/* FL_FUNCTION_H */

