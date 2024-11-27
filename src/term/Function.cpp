/*
fuzzylite (R), a fuzzy logic control library in C++.

Copyright (C) 2010-2024 FuzzyLite Limited. All rights reserved.
Author: Juan Rada-Vilela, PhD <jcrada@fuzzylite.com>.

This file is part of fuzzylite.

fuzzylite is free software: you can redistribute it and/or modify it under
the terms of the FuzzyLite License included with the software.

You should have received a copy of the FuzzyLite License along with
fuzzylite. If not, see <https://github.com/fuzzylite/fuzzylite/>.

fuzzylite is a registered trademark of FuzzyLite Limited.
*/

#include "fuzzylite/term/Function.h"

#include <queue>
#include <stack>

#include "fuzzylite/Engine.h"
#include "fuzzylite/factory/FactoryManager.h"
#include "fuzzylite/rule/Rule.h"
#include "fuzzylite/variable/InputVariable.h"
#include "fuzzylite/variable/OutputVariable.h"

namespace fuzzylite {

    /**
     * Parsing elements
     */

    Function::Element::Element(const std::string& name, const std::string& description, Type type) :
        name(name),
        description(description),
        type(type),
        unary(fl::null),
        binary(fl::null),
        arity(0),
        precedence(0),
        associativity(-1) {}

    Function::Element::Element(
        const std::string& name,
        const std::string& description,
        Type type,
        Unary unary,
        int precedence,
        int associativity
    ) :
        name(name),
        description(description),
        type(type),
        unary(unary),
        binary(fl::null),
        arity(1),
        precedence(precedence),
        associativity(associativity) {}

    Function::Element::Element(
        const std::string& name,
        const std::string& description,
        Type type,
        Binary binary,
        int precedence,
        int associativity
    ) :
        name(name),
        description(description),
        type(type),
        unary(fl::null),
        binary(binary),
        arity(2),
        precedence(precedence),
        associativity(associativity) {}

    Function::Element::~Element() {}

    bool Function::Element::isOperator() const {
        return type == Operator;
    }

    bool Function::Element::isFunction() const {
        return type == Function;
    }

    Function::Element* Function::Element::clone() const {
        return new Element(*this);
    }

    std::string Function::Element::toString() const {
        std::ostringstream ss;

        if (type == Operator) {
            ss << "Operator (name=" << name << ", " << "description=" << description << ", "
               << "precedence=" << precedence << ", " << "arity=" << arity << ", " << "associativity=" << associativity
               << ", ";
            if (arity == 1)
                ss << "pointer=unary";
            else if (arity == 2)
                ss << "pointer=binary";
            else
                ss << "pointer=arity(" << arity << ")";
            ss << ")";
        } else if (type == Function) {
            ss << "Function (name=" << name << ", " << "description=" << description << ", " << "arity=" << arity
               << ", " << "associativity=" << associativity << ", ";
            if (arity == 1)
                ss << "pointer=unary";
            else if (arity == 2)
                ss << "pointer=binary";
            else
                ss << "pointer=arity(" << arity << ")";
            ss << ")";
        }
        return ss.str();
    }

    /******************************
     * Tree Node Elements
     ******************************/

    Function::Node::Node(Element* element, Node* left, Node* right) :
        element(element),
        left(left),
        right(right),
        variable(""),
        constant(fl::nan) {}

    Function::Node::Node(const std::string& variable) :
        element(fl::null),
        left(fl::null),
        right(fl::null),
        variable(variable),
        constant(fl::nan) {}

    Function::Node::Node(scalar constant) :
        element(fl::null),
        left(fl::null),
        right(fl::null),
        variable(""),
        constant(constant) {}

    Function::Node::Node(const Node& other) :
        element(fl::null),
        left(fl::null),
        right(fl::null),
        variable(""),
        constant(fl::nan) {
        copyFrom(other);
    }

    Function::Node& Function::Node::operator=(const Node& other) {
        if (this != &other) {
            element.reset(fl::null);
            left.reset(fl::null);
            right.reset(fl::null);

            copyFrom(other);
        }
        return *this;
    }

    void Function::Node::copyFrom(const Node& other) {
        if (other.element.get())
            element.reset(other.element->clone());
        if (other.left.get())
            left.reset(other.left->clone());
        if (other.right.get())
            right.reset(other.right->clone());
        variable = other.variable;
        constant = other.constant;
    }

    Function::Node::~Node() {}

    std::string Function::Node::value() const {
        std::ostringstream ss;
        if (element.get())
            ss << element->name;
        else if (not variable.empty())
            ss << variable;
        else
            ss << Op::str(constant);
        return ss.str();
    }

    scalar Function::Node::evaluate(const std::map<std::string, scalar>* variables) const {
        scalar result = fl::nan;
        if (element.get()) {
            if (element->unary) {
                if (left.get() or right.get())
                    result = element->unary((left.get() ? left : right)->evaluate(variables));
                else
                    throw Exception("[function error] expected one node, but got none: " + toString());
            } else if (element->binary) {
                if (left.get() and right.get())
                    result = element->binary(left->evaluate(variables), right->evaluate(variables));
                else
                    throw Exception("[function error] expected two nodes, but got fewer: " + toString());
            } else {
                std::ostringstream ex;
                ex << "[function error] expected a pointer to a unary or binary function in node <" << element->name
                   << ">, but got none";
                throw Exception(ex.str(), FL_AT);
            }

        } else if (not variable.empty()) {
            if (not variables) {
                throw Exception(
                    "[function error] expected a map of variables containing the value for '" + variable
                        + "', but none was provided",
                    FL_AT
                );
            }
            std::map<std::string, scalar>::const_iterator it = variables->find(variable);
            if (it != variables->end())
                result = it->second;
            else
                throw Exception("[function error] unknown variable <" + variable + ">", FL_AT);
        } else {
            result = constant;
        }
        return result;
    }

    std::size_t Function::Node::treeSize(const Node* root) const {
        if (not root)
            root = this;
        std::size_t result = 0;
        if (root->left.get())
            result += treeSize(root->left.get());
        if (root->right.get())
            result += treeSize(root->right.get());
        if (root->element.get())
            result += 1;
        return result;
    }

    std::size_t Function::Node::treeSize(Element::Type type, const Node* root) const {
        if (not root)
            root = this;
        std::size_t result = 0;
        if (root->left.get())
            result += treeSize(type, root->left.get());
        if (root->right.get())
            result += treeSize(type, root->right.get());
        if (root->element.get() and root->element->type == type)
            result += 1;
        return result;
    }

    Function::Node* Function::Node::clone() const {
        return new Node(*this);
    }

    std::string Function::Node::toString() const {
        return value();
    }

    std::string Function::Node::toPrefix(const Node* node) const {
        if (not node)
            node = this;
        if (not Op::isNaN(node->constant))  // is terminal
            return Op::str(node->constant);
        if (not node->variable.empty())
            return node->variable;

        std::ostringstream ss;
        ss << node->toString();
        if (node->left.get())
            ss << " " << this->toPrefix(node->left.get());
        if (node->right.get())
            ss << " " << this->toPrefix(node->right.get());
        return ss.str();
    }

    std::string Function::Node::toInfix(const Node* node) const {
        if (not node)
            node = this;
        if (not Op::isNaN(node->constant))  // is proposition
            return Op::str(node->constant);
        if (not node->variable.empty())
            return node->variable;

        std::vector<std::string> children;
        if (node->left.get())
            children.push_back(this->toInfix(node->left.get()));
        if (node->right.get())
            children.push_back(this->toInfix(node->right.get()));
        const bool isFunction = node->element.get() and node->element->isFunction();
        std::string result;
        if (isFunction)
            result = node->value() + " ( " + Op::join(children, " ") + " )";
        else  // TODO: Add parentheses to operations
            if (children.size() == 1)
                result = node->value() + " " + children.front();
            else
                result = Op::join(children, " " + node->value() + " ");
        return result;
    }

    std::string Function::Node::toPostfix(const Node* node) const {
        if (not node)
            node = this;
        if (not Op::isNaN(node->constant))  // is proposition
            return Op::str(node->constant);
        if (not node->variable.empty())
            return node->variable;

        std::ostringstream ss;
        if (node->left.get())
            ss << this->toPostfix(node->left.get()) << " ";
        if (node->right.get())
            ss << this->toPostfix(node->right.get()) << " ";
        ss << node->toString();
        return ss.str();
    }

    /**********************************
     * Function class.
     **********************************/
    Function::Function(
        const std::string& name,
        const std::string& formula,
        // todo: this should be a std::unordered_map
        const std::map<std::string, scalar>& variables,
        // todo: this should be a std::shared_ptr
        const Engine* engine,
        bool load
    ) :
        Term(name),
        _root(fl::null),
        _formula(formula),
        _engine(engine),
        variables(variables) {
        if (load)
            this->load();
    }

    Function::Function(const std::string& name, const std::string& formula, const Engine* engine) :
        Term(name),
        _root(fl::null),
        _formula(formula),
        _engine(engine) {}

    Function::Function(const Function& other) :
        Term(other),
        _root(fl::null),
        _formula(other._formula),
        _engine(other._engine) {
        if (other._root.get())
            _root.reset(other._root->clone());
        variables = other.variables;
    }

    Function& Function::operator=(const Function& other) {
        if (this != &other) {
            _root.reset(fl::null);

            Term::operator=(other);
            _formula = other._formula;
            _engine = other._engine;
            if (other._root.get())
                _root.reset(other._root->clone());
            variables = other.variables;
        }
        return *this;
    }

    Function::~Function() {}

    std::string Function::className() const {
        return "Function";
    }

    scalar Function::membership(scalar x) const {
        if (not _root.get())
            throw Exception("[function error] function <" + _formula + "> not loaded.", FL_AT);
        if (_engine) {
            for (std::size_t i = 0; i < _engine->numberOfInputVariables(); ++i) {
                InputVariable* input = _engine->getInputVariable(i);
                this->variables[input->getName()] = input->getValue();
            }
            for (std::size_t i = 0; i < _engine->numberOfOutputVariables(); ++i) {
                OutputVariable* output = _engine->getOutputVariable(i);
                this->variables[output->getName()] = output->getValue();
            }
        }
        this->variables["x"] = x;
        return this->evaluate(&this->variables);
    }

    scalar Function::evaluate(const std::map<std::string, scalar>& localVariables) const {
        return evaluate(&localVariables);
    }

    scalar Function::evaluate(const std::map<std::string, scalar>* localVariables) const {
        if (not _root.get())
            throw Exception("[function error] function <" + _formula + "> not loaded.", FL_AT);
        if (localVariables)
            for (std::map<std::string, scalar>::const_iterator it = localVariables->begin();
                 it != localVariables->end();
                 ++it)
                variables[it->first] = it->second;
        return _root->evaluate(&this->variables);
    }

    std::string Function::parameters() const {
        return getFormula();
    }

    void Function::configure(const std::string& parameters) {
        load(parameters);
    }

    Function* Function::create(const std::string& name, const std::string& infix, const Engine* engine) {
        FL_unique_ptr<Function> result(new Function(name));
        result->load(infix, engine);
        return result.release();
    }

    bool Function::isLoaded() const {
        return this->_root.get() != fl::null;
    }

    void Function::unload() {
        this->_root.reset(fl::null);
        this->variables.clear();
    }

    void Function::load() {
        load(getFormula());
    }

    void Function::load(const std::string& formula) {
        load(formula, getEngine());
    }

    void Function::load(const std::string& formula, const Engine* engine) {
        setFormula(formula);
        setEngine(engine);
        this->_root.reset(parse(formula));
        // TODO: Remove execution of membership because it does not allow to pass variables different from x
        membership(0.0);  // make sure function evaluates without throwing exception.
    }

    void Function::setFormula(const std::string& formula) {
        this->_formula = formula;
    }

    std::string Function::getFormula() const {
        return this->_formula;
    }

    void Function::setEngine(const Engine* engine) {
        this->_engine = engine;
    }

    const Engine* Function::getEngine() const {
        return this->_engine;
    }

    Function::Node* Function::root() const {
        return this->_root.get();
    }

    Function* Function::clone() const {
        return new Function(*this);
    }

    Term* Function::constructor() {
        return new Function;
    }

    void Function::updateReference(const Engine* engine) {
        setEngine(engine);
        try {
            load();
        } catch (...) {
            // ignore
        }
    }

    std::string Function::space(const std::string& formula) const {
        std::vector<std::string> chars;
        chars.push_back("(");
        chars.push_back(")");
        chars.push_back(",");

        std::vector<std::string> operators = FactoryManager::instance()->function()->availableOperators();
        for (std::size_t i = 0; i < operators.size(); ++i)
            if (not(operators.at(i) == Rule::andKeyword() or operators.at(i) == Rule::orKeyword()))
                chars.push_back(operators.at(i));

        std::string result = formula;
        for (std::size_t i = 0; i < chars.size(); ++i)
            result = Op::findReplace(result, chars.at(i), " " + chars.at(i) + " ");
        return result;
    }

    /****************************************
     * The Glorious Parser
     * Shunting-yard algorithm
     * @todo: maybe change it for http://en.wikipedia.org/wiki/Operator-precedence_parser
     ***************************************/

    std::string Function::toPostfix(const std::string& formula) const {
        std::string spacedFormula = space(formula);

        std::queue<std::string> queue;
        std::stack<std::string> stack;

        std::stringstream tokenizer(spacedFormula);
        std::string token;
        FunctionFactory* factory = FactoryManager::instance()->function();
        while (tokenizer >> token) {
            Element* element = factory->getObject(token);
            bool isOperand = not element and token != "(" and token != ")" and token != ",";

            if (isOperand) {
                queue.push(token);

            } else if (element and element->isFunction()) {
                stack.push(token);

            } else if (token == ",") {
                while (not stack.empty() and stack.top() != "(") {
                    queue.push(stack.top());
                    stack.pop();
                }
                if (stack.empty() or stack.top() != "(") {
                    std::ostringstream ex;
                    ex << "[parsing error] mismatching parentheses in: " << formula;
                    throw Exception(ex.str(), FL_AT);
                }

            } else if (element and element->isOperator()) {
                Element* op1 = element;
                for (;;) {
                    Element* op2 = fl::null;
                    if (not stack.empty())
                        op2 = factory->getObject(stack.top());
                    if (not op2)
                        break;

                    if ((op1->associativity < 0 and op1->precedence == op2->precedence)
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
                    ex << "[parsing error] mismatching parentheses in: " << formula;
                    throw Exception(ex.str(), FL_AT);
                }
                stack.pop();  // get rid of "("

                Element* top = fl::null;
                if (not stack.empty())
                    top = factory->getObject(stack.top());
                if (top and top->isFunction()) {
                    queue.push(stack.top());
                    stack.pop();
                }
            } else {
                std::ostringstream ex;
                ex << "[parsing error] unexpected error with token <" << token << ">";
                throw Exception(ex.str(), FL_AT);
            }
        }

        while (not stack.empty()) {
            if (stack.top() == "(" or stack.top() == ")") {
                std::ostringstream ex;
                ex << "[parsing error] mismatching parentheses in: " << formula;
                throw Exception(ex.str(), FL_AT);
            }
            queue.push(stack.top());
            stack.pop();
        }

        std::stringstream ssPostfix;
        while (not queue.empty()) {
            ssPostfix << queue.front();
            queue.pop();
            if (not queue.empty())
                ssPostfix << " ";
        }
        return ssPostfix.str();
    }

    Function::Node* Function::parse(const std::string& formula) {
        if (formula.empty())
            throw Exception("[function error] formula is empty", FL_AT);
        std::string postfix = toPostfix(formula);

        std::stack<Node*> stack;

        std::istringstream tokenizer(postfix);
        std::string token;
        FunctionFactory* factory = FactoryManager::instance()->function();
        while (tokenizer >> token) {
            Element* element = factory->getObject(token);
            bool isOperand = not element and token != "(" and token != ")" and token != ",";

            if (element) {
                if (element->arity > static_cast<int>(stack.size())) {
                    std::ostringstream ss;
                    ss << "[function error] " << (element->isOperator() ? "operator" : "function") << " <"
                       << element->name << "> has arity <" << element->arity
                       << ">, "
                          "but found <"
                       << stack.size() << "> element" << (stack.size() == 1 ? "" : "s");
                    throw Exception(ss.str(), FL_AT);
                }

                Node* node = new Node(element->clone());
                node->right.reset(stack.top());
                stack.pop();
                if (element->arity == 2) {
                    node->left.reset(stack.top());
                    stack.pop();
                }
                stack.push(node);

            } else if (isOperand) {
                Node* node;
                try {
                    scalar value = Op::toScalar(token);
                    node = new Node(value);
                } catch (std::exception& ex) {
                    FL_IUNUSED(ex);
                    node = new Node(token);
                }
                stack.push(node);
            }
        }

        if (stack.size() != 1)
            throw Exception("[function error] ill-formed formula <" + formula + ">", FL_AT);

        return stack.top();
    }

}
