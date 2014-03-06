// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE
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
            ~Node();

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

    private:
        FL_DISABLE_COPY(Function)

    };

}

#endif	/* FL_FUNCTION_H */

