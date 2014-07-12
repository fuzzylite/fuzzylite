/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FL_FUNCTION_H
#define FL_FUNCTION_H

#include "fl/term/Term.h"

#include <map>
#include <string>

namespace fl {

    class Engine;

    class FL_EXPORT Function : public Term {
        /****************************
         * Parsing Elements
         ****************************/

    public:
        typedef scalar(*Unary)(scalar);
        typedef scalar(*Binary)(scalar, scalar);

        struct FL_EXPORT Element {

            enum Type {
                OPERATOR, FUNCTION
            };
            std::string name;
            std::string description;
            Type type;
            Unary unary;
            Binary binary;
            int arity;
            int precedence; //Operator
            int associativity;
            Element(const std::string& name, const std::string& description, Type type);
            Element(const std::string& name, const std::string& description,
                    Type type, Unary unary, int precedence = 0, int associativity = -1);
            Element(const std::string& name, const std::string& description,
                    Type type, Binary binary, int precedence = 0, int associativity = -1);
            virtual ~Element();

            virtual bool isOperator() const;
            virtual bool isFunction() const;

            virtual Element* clone() const;

            virtual std::string toString() const;

        };

        /**************************
         * Tree elements, wrap Elements into Nodes.
         **************************/

        struct FL_EXPORT Node {
            Element* element;
            Node* left;
            Node* right;
            std::string variable;
            scalar value;

            Node(Element* element, Node* left = NULL, Node* right = NULL);
            Node(const std::string& variable);
            Node(scalar value);
            Node(const Node& source);
            Node& operator=(const Node& rhs);
            virtual ~Node();

            virtual scalar evaluate(const std::map<std::string, scalar>*
                    variables = NULL) const;

            virtual Node* clone() const;

            virtual std::string toString() const;
            virtual std::string toPrefix(const Node* node = NULL) const;
            virtual std::string toInfix(const Node* node = NULL) const;
            virtual std::string toPostfix(const Node* node = NULL) const;
        private:
            void copyFrom(const Node& source);
        };




        /******************************
         * Term
         ******************************/

    protected:
        Node* _root;
        std::string _formula;
        const Engine* _engine;
    public:
        mutable std::map<std::string, scalar> variables;
        Function(const std::string& name = "",
                const std::string& formula = "", const Engine* engine = NULL);
        Function(const Function& source);
        Function& operator=(const Function& rhs);
        virtual ~Function();

        static Function* create(const std::string& name,
                const std::string& formula,
                const Engine* engine = NULL); // throw (fl::Exception);

        virtual scalar membership(scalar x) const;

        virtual scalar evaluate(const std::map<std::string, scalar>* variables) const;

        virtual std::string className() const;
        virtual std::string parameters() const;
        virtual void configure(const std::string& parameters);

        virtual void setFormula(const std::string& formula);
        virtual std::string getFormula() const;

        virtual void setEngine(const Engine* engine);
        virtual const Engine* getEngine() const;

        virtual Node* root() const;

        virtual bool isLoaded() const;
        virtual void unload();
        virtual void load(); // throw (fl::Exception);
        virtual void load(const std::string& formula); // throw (fl::Exception);
        virtual void load(const std::string& formula, const Engine* engine); // throw (fl::Exception);

        virtual Node* parse(const std::string& formula); // throw (fl::Exception);

        virtual std::string toPostfix(const std::string& formula) const; //throw (fl::Exception);

        virtual std::string space(const std::string& formula) const;

        virtual Function* clone() const;

        static Term* constructor();

        static void main();

    };

}

#endif  /* FL_FUNCTION_H */

