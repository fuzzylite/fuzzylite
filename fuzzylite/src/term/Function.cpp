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
 * File:   Function.cpp
 * Author: jcrada
 *
 * Created on 7 April 2013, 10:17 PM
 */

#include "fl/term/Function.h"

#include "fl/Engine.h"
#include "fl/factory/FunctionFactory.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/rule/Rule.h"

#include <functional>
#include <queue>
#include <stack>
#include <signal.h>
#include <cctype>
#include <memory>

namespace fl {

    /**
     * Parsing elements
     */


    Function::Element::Element(const std::string& name, const std::string& description, Type type)
    : name(name), description(description), type(type), unary(NULL), binary(NULL), arity(0),
    precedence(0), associativity(-1) {

    }

    Function::Element::Element(const std::string& name, const std::string& description,
            Type type, Unary unary, int precedence, int associativity)
    : name(name), description(description), type(type), unary(unary), binary(NULL), arity(1),
    precedence(precedence), associativity(associativity) {
    }

    Function::Element::Element(const std::string& name, const std::string& description,
            Type type, Binary binary, int precedence, int associativity)
    : name(name), description(description), type(type), unary(NULL), binary(binary), arity(2),
    precedence(precedence), associativity(associativity) {
    }

    Function::Element::~Element() {

    }

    bool Function::Element::isOperator() const {
        return type == OPERATOR;
    }

    bool Function::Element::isFunction() const {
        return type == FUNCTION;
    }

    Function::Element* Function::Element::clone() const {
        return new Element(*this);
    }

    std::string Function::Element::toString() const {
        std::ostringstream ss;

        if (type == OPERATOR) {
            ss << "Operator (name=" << name << ", "
                    << "description=" << description << ", "
                    << "precedence=" << precedence << ", "
                    << "arity=" << arity << ", "
                    << "associativity=" << associativity << ", ";
            if (arity == 1) ss << "pointer=" << unary;
            else if (arity == 2) ss << "pointer=" << binary;
            else ss << "pointer=error";
            ss << ")";
        } else if (type == FUNCTION) {
            ss << "MathFunction (name=" << name << ", "
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
    : element(element), left(left), right(right), variable(""), value(fl::nan) {
    }

    Function::Node::Node(const std::string& variable)
    : element(NULL), left(NULL), right(NULL), variable(variable), value(fl::nan) {
    }

    Function::Node::Node(scalar value)
    : element(NULL), left(NULL), right(NULL), variable(""), value(value) {
    }

    Function::Node::Node(const Node& source)
    : element(NULL), left(NULL), right(NULL), variable(""), value(fl::nan) {
        copyFrom(source);
    }

    Function::Node& Function::Node::operator =(const Node& rhs) {
        if (this == &rhs) return *this;

        if (element) delete element;
        if (left) delete left;
        if (right) delete right;

        element = NULL;
        left = NULL;
        right = NULL;

        copyFrom(rhs);
        return *this;
    }

    void Function::Node::copyFrom(const Node& source) {
        if (source.element) element = source.element->clone();
        if (source.left) left = source.left->clone();
        if (source.right) right = source.right->clone();
        variable = source.variable;
        value = source.value;
    }

    Function::Node::~Node() {
        if (element) delete element;
        if (left) delete left;
        if (right) delete right;
    }

    scalar Function::Node::evaluate(const std::map<std::string, scalar>* variables) const {
        scalar result = fl::nan;
        if (element) {
            if (element->unary) {
                result = element->unary(left->evaluate(variables));
            } else if (element->binary) {
                result = element->binary(right->evaluate(variables), left->evaluate(variables));
            } else {
                std::ostringstream ex;
                ex << "[function error] arity <" << element->arity << "> of "
                        << (element->isOperator() ? "operator" : "function") <<
                        " <" << element->name << "> is NULL";
                throw fl::Exception(ex.str(), FL_AT);
            }

        } else if (not variable.empty()) {
            if (not variables) {
                throw fl::Exception("[function error] "
                        "expected a map of variables, but none was provided", FL_AT);
            }
            std::map<std::string, scalar>::const_iterator it = variables->find(variable);
            if (it != variables->end()) result = it->second;
            else throw fl::Exception("[function error] "
                    "unknown variable <" + variable + ">", FL_AT);
        } else {
            result = value;
        }
        return result;
    }

    Function::Node* Function::Node::clone() const {
        return new Node(*this);
    }

    std::string Function::Node::toString() const {
        std::ostringstream ss;
        if (element) ss << element->name;
        else if (not variable.empty()) ss << variable;
        else ss << fl::Op::str(value);
        return ss.str();
    }

    std::string Function::Node::toPrefix(const Node* node) const {
        if (not node) node = this;
        if (not fl::Op::isNaN(node->value)) { //is terminal
            return fl::Op::str(node->value);
        }
        if (not node->variable.empty()) {
            return node->variable;
        }

        std::ostringstream ss;
        ss << node->toString();
        if (node->left)
            ss << " " << this->toPrefix(node->left);
        if (node->right)
            ss << " " << this->toPrefix(node->right);
        return ss.str();
    }

    std::string Function::Node::toInfix(const Node* node) const {
        if (not node) node = this;
        if (not fl::Op::isNaN(node->value)) { //is proposition
            return fl::Op::str(node->value);
        }
        if (not node->variable.empty()) {
            return node->variable;
        }

        std::ostringstream ss;
        if (node->left)
            ss << this->toInfix(node->left) << " ";
        ss << node->toString();
        if (node->right)
            ss << " " << this->toInfix(node->right);
        return ss.str();
    }

    std::string Function::Node::toPostfix(const Node* node) const {
        if (not node) node = this;
        if (not fl::Op::isNaN(node->value)) { //is proposition
            return fl::Op::str(node->value);
        }
        if (not node->variable.empty()) {
            return node->variable;
        }

        std::ostringstream ss;
        if (node->left)
            ss << this->toPostfix(node->left) << " ";
        if (node->right)
            ss << this->toPostfix(node->right) << " ";
        ss << node->toString();
        return ss.str();
    }

    /**********************************
     * Function class.
     **********************************/
    Function::Function(const std::string& name,
            const std::string& formula, const Engine* engine)
    : Term(name), _formula(formula), _engine(engine), root(NULL) {
    }

    Function::Function(const Function& source) : Term(source),
    _formula(source._formula), _engine(source._engine), root(NULL) {
        if (source.root) root = source.root->clone();
    }

    Function& Function::operator =(const Function& rhs) {
        if (this == &rhs) return *this;
        if (root) delete root;
        root = NULL;

        Term::operator =(rhs);
        _formula = rhs._formula;
        _engine = rhs._engine;
        if (rhs.root) root = rhs.root->clone();
        return *this;
    }

    Function::~Function() {
        if (this->root) delete this->root;
    }

    std::string Function::className() const {
        return "Function";
    }

    scalar Function::membership(scalar x) const {
        if (not this->root) return fl::nan;
        if (this->_engine) {
            for (int i = 0; i < this->_engine->numberOfInputVariables(); ++i) {
                InputVariable* input = this->_engine->getInputVariable(i);
                this->variables[input->getName()] = input->getInputValue();
            }
            for (int i = 0; i < this->_engine->numberOfOutputVariables(); ++i) {
                OutputVariable* output = this->_engine->getOutputVariable(i);
                this->variables[output->getName()] = output->getOutputValue();
            }
        }
        this->variables["x"] = x;
        return this->evaluate(&this->variables);
    }

    scalar Function::evaluate(const std::map<std::string, scalar>* localVariables) const {
        if (not this->root)
            throw fl::Exception("[function error] evaluation failed because the function is not loaded", FL_AT);
        if (localVariables)
            return this->root->evaluate(localVariables);
        return this->root->evaluate(&this->variables);
    }

    std::string Function::parameters() const {
        return _formula;
    }

    void Function::configure(const std::string& parameters) {
        load(parameters);
    }

    Function* Function::create(const std::string& name,
            const std::string& infix, const Engine* engine) throw (fl::Exception) {
        std::auto_ptr<Function> result(new Function(name));
        result->load(infix, engine);
        return result.release();
    }

    void Function::unload() {
        if (this->root) {
            delete this->root;
            this->root = NULL;
        }
    }

    void Function::load() throw (fl::Exception) {
        load(this->_formula);
    }

    void Function::load(const std::string& formula) throw (fl::Exception) {
        load(formula, this->_engine);
    }

    void Function::load(const std::string& formula,
            const Engine* engine) throw (fl::Exception) {
        unload();
        this->_formula = formula;
        this->_engine = engine;
        std::auto_ptr<Node> node(parse(formula));
        this->root = node.release();
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

    Function* Function::clone() const {
        return new Function(*this);
    }

    Term* Function::constructor() {
        return new Function;
    }

    std::string Function::space(const std::string& formula) const {
        std::vector<std::string> chars;
        chars.push_back("(");
        chars.push_back(")");
        chars.push_back(",");

        std::vector<std::string> operators = fl::FactoryManager::instance()->function()->availableOperators();
        for (std::size_t i = 0; i < operators.size(); ++i) {
            if (not (operators.at(i) == fl::Rule::andKeyword() or
                    operators.at(i) == fl::Rule::orKeyword())) {
                chars.push_back(operators.at(i));
            }
        }

        std::string result = formula;
        for (std::size_t i = 0; i < chars.size(); ++i) {
            result = fl::Op::findReplace(result, chars.at(i), " " + chars.at(i) + " ");
        }
        return result;
    }

    /****************************************
     * The Glorious Parser
     * Shunting-yard algorithm
     * TODO: Maybe change it for http://en.wikipedia.org/wiki/Operator-precedence_parser
     ***************************************/

    std::string Function::toPostfix(const std::string& formula) const throw (fl::Exception) {
        std::string spacedFormula = space(formula);

        std::queue<std::string> queue;
        std::stack<std::string> stack;

        std::stringstream tokenizer(spacedFormula);
        std::string token;
        FunctionFactory* factory = fl::FactoryManager::instance()->function();
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
                    throw fl::Exception(ex.str(), FL_AT);
                }

            } else if (element and element->isOperator()) {
                Element* op1 = element;
                while (true) {
                    Element* op2 = NULL;
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
                    throw fl::Exception(ex.str(), FL_AT);
                }
                stack.pop(); //get rid of "("

                Element* top = NULL;
                if (not stack.empty()) top = factory->getObject(stack.top());
                if (top and top->isFunction()) {
                    queue.push(stack.top());
                    stack.pop();
                }
            } else {
                std::ostringstream ex;
                ex << "[parsing error] unexpected error with token <" << token << ">";
                throw fl::Exception(ex.str(), FL_AT);
            }
        }

        while (not stack.empty()) {
            if (stack.top() == "(" or stack.top() == ")") {
                std::ostringstream ex;
                ex << "[parsing error] mismatching parentheses in: " << formula;
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
        //        FL_DBG("postfix=" << ssPostfix.str());
        return ssPostfix.str();
    }

    //    bool FunctionFactory::isOperand(const std::string& name) const {
    //        //An operand is not a parenthesis...
    //        if (name == "(" or name == ")" or name == ",") return false;
    //        //nor an operator...
    //        if (isOperator(name)) return false;
    //        //nor a function...
    //        if (isFunction(name)) return false;
    //        //...it is everything else :)
    //        return true;
    //    }

    Function::Node* Function::parse(const std::string& formula) throw (fl::Exception) {
        if (formula.empty())
            throw fl::Exception("[function error] formula is empty", FL_AT);
        std::string postfix = toPostfix(formula);

        std::stack<Node*> stack;

        std::istringstream tokenizer(postfix);
        std::string token;
        FunctionFactory* factory = fl::FactoryManager::instance()->function();
        while (tokenizer >> token) {
            Element* element = factory->getObject(token);
            bool isOperand = not element and token != "(" and token != ")" and token != ",";

            if (element) {
                if (element->arity > (int) stack.size()) {
                    std::ostringstream ss;
                    ss << "[function error] " << (element->isOperator() ? "operator" : "function") <<
                            " <" << element->name << "> has arity <" << element->arity << ">, "
                            "but found <" << stack.size() << "> element" <<
                            (stack.size() == 1 ? "" : "s");
                    throw fl::Exception(ss.str(), FL_AT);
                }

                Node* node = new Node(element->clone());
                node->left = stack.top();
                stack.pop();
                if (element->arity == 2) {
                    node->right = stack.top();
                    stack.pop();
                }
                stack.push(node);

            } else if (isOperand) {
                Node* node;
                try {
                    scalar value = fl::Op::toScalar(token, false);
                    node = new Node(value);
                } catch (std::exception& ex) {
                    (void) ex;
                    node = new Node(token);
                }
                stack.push(node);
            }
        }

        if (stack.size() != 1)
            throw fl::Exception("[function error] ill-formed formula <" + formula + ">", FL_AT);

        return stack.top();
    }

    void Function::main() {
        Function f;
        std::string text = "3+4*2/(1-5)^2^3";
        FL_LOG(f.toPostfix(text));
        FL_LOG("P: " << f.parse(text)->toInfix());
        FL_LOG(">" << f.parse(text)->evaluate());
        //3 4 2 * 1 5 - 2 3 ^ ^ / +

        f.variables["y"] = 1.0;
        text = "sin(y*x)^2/x";
        FL_LOG("pre: " << f.parse(text)->toPrefix());
        FL_LOG("in: " << f.parse(text)->toInfix());
        FL_LOG("pos: " << f.parse(text)->toPostfix());
        f.load(text);
        FL_LOG("Result: " << f.membership(1));
        //y x * sin 2 ^ x /


        text = "(Temperature is High and Oxigen is Low) or "
                "(Temperature is Low and (Oxigen is Low or Oxigen is High))";
        FL_LOG(f.toPostfix(text));

        f.variables["pi"] = 3.14;
        text = "-5 *4/sin(-pi/2)";
        FL_LOG(f.toPostfix(text));
        try {
            FL_LOG(f.parse(text)->evaluate());
        } catch (std::exception& e) {
            FL_LOG(e.what());
        }
        f.variables["pi"] = 3.14;
        text = "~5 *4/sin(~pi/2)";
        FL_LOG(f.toPostfix(text));
        try {
            FL_LOG(f.parse(text)->evaluate(&f.variables));
        } catch (std::exception& e) {
            FL_LOG(e.what());
        }
    }


}