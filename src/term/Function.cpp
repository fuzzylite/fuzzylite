/*
 fuzzylite (R), a fuzzy logic control library in C++.
 Copyright (C) 2010-2017 FuzzyLite Limited. All rights reserved.
 Author: Juan Rada-Vilela, Ph.D. <jcrada@fuzzylite.com>

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with
 fuzzylite. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite is a registered trademark of FuzzyLite Limited.
 */

#include "fl/term/Function.h"

#include "fl/Engine.h"
#include "fl/factory/FactoryManager.h"
#include "fl/rule/Rule.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"

#include <queue>
#include <stack>

namespace fl {

    /**
     * Parsing elements
     */


    Function::Element::Element(const std::string& name, const std::string& description, Type type)
    : name(name), description(description), type(type), unary(fl::null), binary(fl::null), arity(0),
    precedence(0), associativity(-1) { }

    Function::Element::Element(const std::string& name, const std::string& description,
            Type type, Unary unary, int precedence, int associativity)
    : name(name), description(description), type(type), unary(unary), binary(fl::null), arity(1),
    precedence(precedence), associativity(associativity) { }

    Function::Element::Element(const std::string& name, const std::string& description,
            Type type, Binary binary, int precedence, int associativity)
    : name(name), description(description), type(type), unary(fl::null), binary(binary), arity(2),
    precedence(precedence), associativity(associativity) { }

    Function::Element::~Element() { }

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
            ss << "Operator (name=" << name << ", "
                    << "description=" << description << ", "
                    << "precedence=" << precedence << ", "
                    << "arity=" << arity << ", "
                    << "associativity=" << associativity << ", ";
            if (arity == 1) ss << "pointer=" << unary;
            else if (arity == 2) ss << "pointer=" << binary;
            else ss << "pointer=error";
            ss << ")";
        } else if (type == Function) {
            ss << "Function (name=" << name << ", "
                    << "description=" << description << ", "
                    << "arity=" << arity << ", "
                    << "associativity=" << associativity << ", ";
            if (arity == 1) ss << "pointer=" << unary;
            else if (arity == 2) ss << "pointer=" << binary;
            else ss << "pointer=error";
            ss << ")";
        }
        return ss.str();
    }

    /******************************
     * Tree Node Elements
     ******************************/

    Function::Node::Node(Element* element, Node* left, Node* right)
    : element(element), left(left), right(right), variable(""), value(fl::nan) { }

    Function::Node::Node(const std::string& variable)
    : element(fl::null), left(fl::null), right(fl::null), variable(variable), value(fl::nan) { }

    Function::Node::Node(scalar value)
    : element(fl::null), left(fl::null), right(fl::null), variable(""), value(value) { }

    Function::Node::Node(const Node& other)
    : element(fl::null), left(fl::null), right(fl::null), variable(""), value(fl::nan) {
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
        if (other.element.get()) element.reset(other.element->clone());
        if (other.left.get()) left.reset(other.left->clone());
        if (other.right.get()) right.reset(other.right->clone());
        variable = other.variable;
        value = other.value;
    }

    Function::Node::~Node() { }

    scalar Function::Node::evaluate(const std::map<std::string, scalar>* variables) const {
        scalar result = fl::nan;
        if (element.get()) {
            if (element->unary) {
                result = element->unary(left->evaluate(variables));
            } else if (element->binary) {
                result = element->binary(right->evaluate(variables), left->evaluate(variables));
            } else {
                std::ostringstream ex;
                ex << "[function error] arity <" << element->arity << "> of "
                        << (element->isOperator() ? "operator" : "function") <<
                        " <" << element->name << "> is fl::null";
                throw Exception(ex.str(), FL_AT);
            }

        } else if (not variable.empty()) {
            if (not variables) {
                throw Exception("[function error] "
                        "expected a map of variables, but none was provided", FL_AT);
            }
            std::map<std::string, scalar>::const_iterator it = variables->find(variable);
            if (it != variables->end()) result = it->second;
            else throw Exception("[function error] "
                    "unknown variable <" + variable + ">", FL_AT);
        } else {
            result = value;
        }
        return result;
    }

    std::size_t Function::Node::treeSize(const Node* root) const {
        if (not root) root = this;
        std::size_t result = 0;
        if (root->left.get()) {
            result += treeSize(root->left.get());
        }
        if (root->right.get()) {
            result += treeSize(root->right.get());
        }
        if (root->element.get()) {
            result += 1;
        }
        return result;
    }

    std::size_t Function::Node::treeSize(Element::Type type, const Node* root) const {
        if (not root) root = this;
        std::size_t result = 0;
        if (root->left.get()) {
            result += treeSize(type, root->left.get());
        }
        if (root->right.get()) {
            result += treeSize(type, root->right.get());
        }
        if (root->element.get() and root->element->type == type) {
            result += 1;
        }
        return result;
    }

    Function::Node* Function::Node::clone() const {
        return new Node(*this);
    }

    std::string Function::Node::toString() const {
        std::ostringstream ss;
        if (element.get()) ss << element->name;
        else if (not variable.empty()) ss << variable;
        else ss << Op::str(value);
        return ss.str();
    }

    std::string Function::Node::toPrefix(const Node* node) const {
        if (not node) node = this;
        if (not Op::isNaN(node->value)) { //is terminal
            return Op::str(node->value);
        }
        if (not node->variable.empty()) {
            return node->variable;
        }

        std::ostringstream ss;
        ss << node->toString();
        if (node->left.get())
            ss << " " << this->toPrefix(node->left.get());
        if (node->right.get())
            ss << " " << this->toPrefix(node->right.get());
        return ss.str();
    }

    std::string Function::Node::toInfix(const Node* node) const {
        if (not node) node = this;
        if (not Op::isNaN(node->value)) { //is proposition
            return Op::str(node->value);
        }
        if (not node->variable.empty()) {
            return node->variable;
        }

        std::ostringstream ss;
        if (node->left.get())
            ss << this->toInfix(node->left.get()) << " ";
        ss << node->toString();
        if (node->right.get())
            ss << " " << this->toInfix(node->right.get());
        return ss.str();
    }

    std::string Function::Node::toPostfix(const Node* node) const {
        if (not node) node = this;
        if (not Op::isNaN(node->value)) { //is proposition
            return Op::str(node->value);
        }
        if (not node->variable.empty()) {
            return node->variable;
        }

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
    Function::Function(const std::string& name,
            const std::string& formula, const Engine* engine)
    : Term(name), _root(fl::null), _formula(formula), _engine(engine) { }

    Function::Function(const Function& other) : Term(other),
    _root(fl::null), _formula(other._formula), _engine(other._engine) {
        if (other._root.get()) _root.reset(other._root->clone());
        variables = other.variables;
    }

    Function& Function::operator=(const Function& other) {
        if (this != &other) {
            _root.reset(fl::null);

            Term::operator=(other);
            _formula = other._formula;
            _engine = other._engine;
            if (other._root.get()) _root.reset(other._root->clone());
            variables = other.variables;
        }
        return *this;
    }

    Function::~Function() { }

    std::string Function::className() const {
        return "Function";
    }

    Complexity Function::complexity() const {
        Complexity result;
        result.comparison(2 + 2); //membership(scalar) + membership(std::map)
        if (_engine) { //insert variables in map
            const std::size_t engineVariables = _engine->variables().size();
            result.function(engineVariables * std::log(scalar(variables.size() + engineVariables)));
            result.function(1 * std::log(scalar(variables.size() + engineVariables)));
        }
        if (_root.get()) {
            //Node::evaluate multiplies by tree size
            const scalar treeSize = scalar(_root->treeSize());
            result.comparison(3 * treeSize); //if element, unary, binary
            result.function(treeSize * std::log(treeSize)); //only operands in tree
        }
        return result;
    }

    scalar Function::membership(scalar x) const {
        if (not _root.get()) {
            throw Exception("[function error] function <" + _formula + "> not loaded.", FL_AT);
        }
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

    scalar Function::evaluate(const std::map<std::string, scalar>* localVariables) const {
        if (not _root.get())
            throw Exception("[function error] evaluation failed because the function is not loaded", FL_AT);
        if (localVariables)
            return _root->evaluate(localVariables);
        return _root->evaluate(&this->variables);
    }

    std::string Function::parameters() const {
        return getFormula();
    }

    void Function::configure(const std::string& parameters) {
        load(parameters);
    }

    Function* Function::create(const std::string& name,
            const std::string& infix, const Engine* engine) {
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

    void Function::load(const std::string& formula,
            const Engine* engine) {
        setFormula(formula);
        setEngine(engine);
        this->_root.reset(parse(formula));
        membership(0.0); //make sure function evaluates without throwing exception.
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
            //ignore
        }
    }

    std::string Function::space(const std::string& formula) const {
        std::vector<std::string> chars;
        chars.push_back("(");
        chars.push_back(")");
        chars.push_back(",");

        std::vector<std::string> operators = FactoryManager::instance()->function()->availableOperators();
        for (std::size_t i = 0; i < operators.size(); ++i) {
            if (not (operators.at(i) == Rule::andKeyword() or
                    operators.at(i) == Rule::orKeyword())) {
                chars.push_back(operators.at(i));
            }
        }

        std::string result = formula;
        for (std::size_t i = 0; i < chars.size(); ++i) {
            result = Op::findReplace(result, chars.at(i), " " + chars.at(i) + " ");
        }
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
                    if (not stack.empty()) op2 = factory->getObject(stack.top());
                    if (not op2) break;

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
                stack.pop(); //get rid of "("

                Element* top = fl::null;
                if (not stack.empty()) top = factory->getObject(stack.top());
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
            if (not queue.empty()) ssPostfix << " ";
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
                if (element->arity > static_cast<int> (stack.size())) {
                    std::ostringstream ss;
                    ss << "[function error] " << (element->isOperator() ? "operator" : "function") <<
                            " <" << element->name << "> has arity <" << element->arity << ">, "
                            "but found <" << stack.size() << "> element" <<
                            (stack.size() == 1 ? "" : "s");
                    throw Exception(ss.str(), FL_AT);
                }

                Node* node = new Node(element->clone());
                node->left.reset(stack.top());
                stack.pop();
                if (element->arity == 2) {
                    node->right.reset(stack.top());
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
