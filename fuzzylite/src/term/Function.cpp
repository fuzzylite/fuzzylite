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
 * File:   Function.cpp
 * Author: jcrada
 *
 * Created on 7 April 2013, 10:17 PM
 */

#include "fl/term/Function.h"

#include "fl/Engine.h"
#include "fl/variable/InputVariable.h"
#include "fl/variable/OutputVariable.h"
#include "fl/rule/Rule.h"

#include <functional>
#include <queue>
#include <stack>
#include <signal.h>
#include <cctype>
namespace fl {

    /**********************************
     * Function class.
     **********************************/
    Function::Function(const std::string& name,
            const std::string& infix, const Engine* engine,
            bool loadBuiltInFunctions)
    : Term(name), _infix(infix), _engine(engine), root(NULL) {
        loadOperators();
        if (loadBuiltInFunctions) {
            this->loadBuiltInFunctions();
        }
    }

    Function::~Function() {
        if (this->root) delete this->root;
        for (std::map<std::string, Operator*>::iterator it = this->operators.begin();
                it != this->operators.end(); ++it) {
            delete it->second;
        }
        for (std::map<std::string, BuiltInFunction*>::iterator it = this->functions.begin();
                it != this->functions.end(); ++it) {
            delete it->second;
        }
    }

    Function* Function::create(const std::string& name,
            const std::string& infix, const Engine* engine) throw (fl::Exception) {
        Function* result = new Function(name);
        try {
            result->load(infix, engine);
        } catch (fl::Exception& ex) {
            delete result;
            throw ex;
        }
        return result;
    }

    scalar Function::membership(scalar x) const {
        if (not this->root) return fl::nan;
        //            throw fl::Exception("[function error] function not loaded", FL_AT);
        if (this->_engine) {
            for (int i = 0; i < this->_engine->numberOfInputVariables(); ++i) {
                InputVariable* input = this->_engine->getInputVariable(i);
                const_cast<Function*> (this)->variables[input->getName()] =
                        input->getInput();
            }
            for (int i = 0; i < this->_engine->numberOfOutputVariables(); ++i) {
                OutputVariable* output = this->_engine->getOutputVariable(i);
                const_cast<Function*> (this)->variables[output->getName()] =
                        output->getLastValidOutput();
            }
        }
        const_cast<Function*> (this)->variables["x"] = x;
        return this->evaluate(&this->variables);
    }

    scalar Function::evaluate(const std::map<std::string, scalar>* localVariables = NULL) const {
        if (not this->root)
            throw fl::Exception("[function error] evaluation failed because function is not loaded", FL_AT);
        if (localVariables)
            return this->root->evaluate(localVariables);
        return this->root->evaluate(&this->variables);
    }

    std::string Function::className() const {
        return "Function";
    }

    std::string Function::toString() const {
        return "Function (" + _infix + ")";
    }

    Function* Function::copy() const {
        Function* result = new Function(this->_name);
        try {
            result->load(this->_infix, this->_engine);
        } catch (fl::Exception& ex) {
            FL_LOG("catched: " << ex.what());
        }
        return result;
    }

    void Function::load() throw (fl::Exception) {
        load(this->_infix, this->_engine);
    }

    void Function::load(const std::string& infix,
            const Engine* engine) throw (fl::Exception) {
        this->root = parse(infix);
        this->_infix = infix;
        this->_engine = engine;
    }

    void Function::setInfix(const std::string& infix) {
        this->_infix = infix;
    }

    std::string Function::getInfix() const {
        return this->_infix;
    }

    void Function::setEngine(const Engine* engine) {
        this->_engine = engine;
    }

    const Engine* Function::getEngine() const {
        return this->_engine;
    }

    /**
     * Parsing elements
     */

    Function::Element::Element(const std::string& name)
    : name(name) { }

    Function::Element::~Element() { }

    Function::Operator::Operator(const std::string& name, Unary unary,
            short precedence, short associativity)
    : Element(name), unary(unary), binary(NULL),
    precedence(precedence), arity(1), associativity(associativity) { }

    Function::Operator::Operator(const std::string& name, Binary binary,
            short precedence, short associativity)
    : Element(name), unary(NULL), binary(binary),
    precedence(precedence), arity(2), associativity(associativity) { }

    std::string Function::Operator::toString() const {
        std::ostringstream ss;
        ss << "Operator (name=" << name << ", "
                << "precedence=" << precedence << ", "
                << "arity=" << arity << ", "
                << "associativity=" << associativity << ", ";
        if (arity == 1) ss << "pointer=" << unary;
        else if (arity == 2) ss << "pointer=" << binary;
        else ss << "pointer=error";
        ss << ")";
        return ss.str();
    }

    Function::BuiltInFunction::BuiltInFunction(const std::string& name,
            Unary unary, short associativity)
    : Element(name), unary(unary), binary(NULL), arity(1), associativity(associativity) { }

    Function::BuiltInFunction::BuiltInFunction(const std::string& name,
            Binary binary, short associativity)
    : Element(name), unary(NULL), binary(binary), arity(2), associativity(associativity) { }

    std::string Function::BuiltInFunction::toString() const {
        std::ostringstream ss;
        ss << "BuiltInFunction (name=" << name << ", "
                << "arity=" << arity << ", "
                << "associativity=" << associativity << ", ";
        if (arity == 1) ss << "pointer=" << unary;
        else if (arity == 2) ss << "pointer=" << binary;
        else ss << "pointer=error";
        ss << ")";
        return ss.str();
    }

    /***********************************
     * Load Operators and Functions
     ***********************************/

    void Function::loadOperators() {
        char p = 7;
        // (!) Logical and (~) Bitwise NOT
        //        this->_unaryOperators["!"] = new Operator("!", std::logical_not<scalar>, p, 1);
        // ~ negates a number
        this->operators["~"] = new Operator("~", fl::Op::negate, p, 1);
        --p; //Power
        this->operators["^"] = new Operator("^", std::pow, p, 1);
        --p; //Multiplication, Division, and Modulo
        this->operators["*"] = new Operator("*", fl::Op::multiplies, p);
        this->operators["/"] = new Operator("/", fl::Op::divides, p);
        this->operators["%"] = new Operator("%", fl::Op::modulus, p);
        --p; //Addition, Subtraction
        this->operators["+"] = new Operator("+", fl::Op::plus, p);
        this->operators["-"] = new Operator("-", fl::Op::minus, p);
        //        --p; //Bitwise AND
        //        this->_binaryOperators["&"] = new Operator("&", std::bit_and, p);
        //        --p; //Bitwise OR
        //        this->_binaryOperators["|"] = new Operator("|", std::bit_or, p);
        --p; //Logical AND
        this->operators[fl::Rule::andKeyword()] =
                new Operator(fl::Rule::andKeyword(), fl::Op::logical_and, p);
        --p; //Logical OR
        this->operators[fl::Rule::orKeyword()] =
                new Operator(fl::Rule::orKeyword(), fl::Op::logical_or, p);
    }

    void Function::loadBuiltInFunctions() {
        this->functions["acos"] = new BuiltInFunction("acos", &(std::acos));
        this->functions["asin"] = new BuiltInFunction("asin", &(std::asin));
        this->functions["atan"] = new BuiltInFunction("atan", &(std::atan));

        this->functions["ceil"] = new BuiltInFunction("ceil", &(std::ceil));
        this->functions["cos"] = new BuiltInFunction("cos", &(std::cos));
        this->functions["cosh"] = new BuiltInFunction("cosh", &(std::cosh));
        this->functions["exp"] = new BuiltInFunction("exp", &(std::exp));
        this->functions["fabs"] = new BuiltInFunction("fabs", &(std::fabs));
        this->functions["floor"] = new BuiltInFunction("floor", &(std::floor));
        this->functions["log"] = new BuiltInFunction("log", &(std::log));
        this->functions["log10"] = new BuiltInFunction("log10", &(std::log10));

        this->functions["sin"] = new BuiltInFunction("sin", &(std::sin));
        this->functions["sinh"] = new BuiltInFunction("sinh", &(std::sinh));
        this->functions["sqrt"] = new BuiltInFunction("sqrt", &(std::sqrt));
        this->functions["tan"] = new BuiltInFunction("tan", &(std::tan));
        this->functions["tanh"] = new BuiltInFunction("tanh", &(std::tanh));

#ifdef FL_UNIX
        //not found in Windows
        this->functions["log1p"] = new BuiltInFunction("log1p", &(log1p));
        this->functions["acosh"] = new BuiltInFunction("acosh", &(acosh));
        this->functions["asinh"] = new BuiltInFunction("asinh", &(asinh));
        this->functions["atanh"] = new BuiltInFunction("atanh", &(atanh));
#endif

        this->functions["pow"] = new BuiltInFunction("pow", &(std::pow));
        this->functions["atan2"] = new BuiltInFunction("atan2", &(std::atan2));
        this->functions["fmod"] = new BuiltInFunction("fmod", &(std::fmod));
    }

    /**
     * Infix to Postfix
     */

    std::string Function::space(const std::string& infix) const {
        std::vector<std::string> chars;
        chars.push_back("(");
        chars.push_back(")");
        chars.push_back(",");

        for (std::map<std::string, Operator*>::const_iterator itOp = this->operators.begin();
                itOp != this->operators.end(); ++itOp) {
            if (itOp->first != fl::Rule::andKeyword() and itOp->first != fl::Rule::orKeyword()) {
                chars.push_back(itOp->first);
            }
        }

        std::string result = infix;
        for (std::size_t i = 0; i < chars.size(); ++i) {
            result = fl::Op::findReplace(result, chars.at(i), " " + chars.at(i) + " ");
        }
        return result;
    }

    std::string Function::toPostfix(const std::string& rawInfix) const throw (fl::Exception) {
        std::string infix = space(rawInfix);
//        FL_DBG("infix=" << infix);

        std::queue<std::string> queue;
        std::stack<std::string> stack;

        std::stringstream tokenizer(infix);
        std::string token;
        while (tokenizer >> token) {
            if (isOperand(token)) {
                queue.push(token);

            } else if (isBuiltInFunction(token)) {
                stack.push(token);

            } else if (token == ",") {
                while (not stack.empty() and stack.top() != "(") {
                    queue.push(stack.top());
                    stack.pop();
                }
                if (stack.empty() or stack.top() != "(") {
                    std::ostringstream ex;
                    ex << "[parsing error] mismatching parentheses in: " << rawInfix;
                    throw fl::Exception(ex.str(), FL_AT);
                }

            } else if (isOperator(token)) {
                Operator* op1 = getOperator(token);
                while (true) {
                    Operator* op2 = NULL;
                    if (not stack.empty() and isOperator(stack.top())) {
                        op2 = this->getOperator(stack.top());
                    } else break;

                    if ((op1->associativity < 0 and op1->precedence <= op2->precedence)
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
                    ex << "[parsing error] mismatching parentheses in: " << rawInfix;
                    throw fl::Exception(ex.str(), FL_AT);
                }
                stack.pop(); //get rid of "("

                if (not stack.empty() and isBuiltInFunction(stack.top())) {
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
                ex << "[parsing error] mismatching parentheses in: " << rawInfix;
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

    Function::Operator* Function::getOperator(const std::string& key) const {
        std::map<std::string, Operator*>::const_iterator it =
                this->operators.find(key);
        if (it == this->operators.end()) return NULL;
        return it->second;
    }

    Function::BuiltInFunction*
    Function::getBuiltInFunction(const std::string& key) const {
        std::map<std::string, BuiltInFunction*>::const_iterator it =
                this->functions.find(key);
        if (it == this->functions.end()) return NULL;
        return it->second;
    }

    bool Function::isOperand(const std::string& name) const {
        //An operand is not a parenthesis...
        if (name == "(" or name == ")" or name == ",") return false;
        //nor an operator...
        if (isOperator(name)) return false;
        //nor a function...
        if (isBuiltInFunction(name)) return false;
        //...it is everything else :)
        return true;
    }

    bool Function::isOperator(const std::string& name) const {
        return operators.find(name) != operators.end();
    }

    bool Function::isBuiltInFunction(const std::string& name) const {
        return functions.find(name) != functions.end();
    }

    /******************************
     * Tree Node Elements
     ******************************/

    Function::Node::Node(Operator* foperator, Node* left, Node* right)
    : foperator(foperator), function(NULL), variable(""), value(fl::nan),
    left(left), right(right) { }

    Function::Node::Node(BuiltInFunction* function, Node* left, Node* right)
    : foperator(NULL), function(function), variable(""), value(fl::nan),
    left(left), right(right) { }

    Function::Node::Node(const std::string& variable)
    : foperator(NULL), function(NULL), variable(variable), value(fl::nan),
    left(NULL), right(NULL) { }

    Function::Node::Node(scalar value)
    : foperator(NULL), function(NULL), variable(""), value(value), left(NULL), right(NULL) { }

    scalar Function::Node::evaluate(const std::map<std::string, scalar>* variables) const {
        scalar result = fl::nan;
        if (foperator) {
            if (foperator->arity == 1) {
                result = foperator->unary(left->evaluate(variables));
            } else if (foperator->arity == 2) {
                result = foperator->binary(right->evaluate(variables), left->evaluate(variables));
            } else {
                throw fl::Exception("[function error] <" + fl::Op::str(foperator->arity) + ">-ary"
                        " operators are not supported, only unary or binary are", FL_AT);
            }
        } else if (function) {
            if (function->arity == 1) {
                result = function->unary(left->evaluate(variables));
            } else if (function->arity == 2) {
                result = function->binary(right->evaluate(variables), left->evaluate(variables));
            } else {
                throw fl::Exception("[function error] <" + fl::Op::str(foperator->arity) + ">-ary"
                        " functions are not supported, only unary or binary are", FL_AT);
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
        FL_DBG(toPostfix() << " = " << result);
        return result;
    }

    std::string Function::Node::toString() const {
        std::ostringstream ss;
        if (foperator) ss << foperator->name;
        else if (function) ss << function->name;
        else if (not variable.empty()) ss << variable;
        else ss << fl::Op::str(value);
        return ss.str();
    }

    std::string Function::Node::toPrefix(const Node* node) const {
        if (not node) node = this;
        if (not fl::Op::isNan(node->value)) { //is terminal
            return fl::Op::str(node->value);
        }
        if (not variable.empty()) {
            return variable;
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
        if (not fl::Op::isNan(node->value)) { //is proposition
            return fl::Op::str(node->value);
        }
        if (not variable.empty()) {
            return variable;
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
        if (not fl::Op::isNan(node->value)) { //is proposition
            return fl::Op::str(node->value);
        }
        if (not variable.empty()) {
            return variable;
        }

        std::ostringstream ss;
        if (node->left)
            ss << this->toPostfix(node->left) << " ";
        if (node->right)
            ss << this->toPostfix(node->right) << " ";
        ss << node->toString();
        return ss.str();
    }

    /****************************************
     * The Glorious Parser
     * Shunting-yard algorithm
     * TODO: Maybe change it for http://en.wikipedia.org/wiki/Operator-precedence_parser
     ***************************************/

    Function::Node* Function::parse(const std::string& infix) throw (fl::Exception) {
        if (infix.empty()) return NULL;
        std::string postfix = toPostfix(infix);

        std::stack<Node*> stack;

        std::istringstream tokenizer(postfix);
        std::string token;
        while (tokenizer >> token) {
            if (isOperator(token)) {
                Operator* op = getOperator(token);
                if (op->arity > (int) stack.size()) {
                    std::ostringstream ss;
                    ss << "[function error] "
                            "operator <" << op->name << "> has arity <" << op->arity << ">, "
                            "but <" << stack.size() << "> element" <<
                            (stack.size() == 1 ? " is " : "s are") <<
                            " available";
                    throw fl::Exception(ss.str(), FL_AT);
                }

                Node* node = new Node(op);
                node->left = stack.top();
                stack.pop();
                if (op->arity == 2) {
                    node->right = stack.top();
                    stack.pop();
                }
                stack.push(node);
            } else if (isBuiltInFunction(token)) {
                BuiltInFunction* function = getBuiltInFunction(token);
                if (function->arity > (int) stack.size()) {
                    std::ostringstream ss;
                    ss << "[function error] "
                            "function <" << function->name << "> has arity <" << function->arity << ">, "
                            "but <" << stack.size() << "> element" <<
                            (stack.size() == 1 ? " is " : "s are") <<
                            " available";
                    throw fl::Exception(ss.str(), FL_AT);
                }

                Node* node = new Node(function);
                node->left = stack.top();
                stack.pop();
                if (function->arity == 2) {
                    node->right = stack.top();
                    stack.pop();
                }
                stack.push(node);

            } else if (isOperand(token)) {
                Node* node;
                try {
                    scalar value = fl::Op::toScalar(token, false);
                    node = new Node(value);
                } catch (fl::Exception& ex) {
                    (void) ex;
                    node = new Node(token);
                }
                stack.push(node);
            }
        }

        if (stack.size() != 1)
            throw fl::Exception("[function error] malformed function <" +
                infix + ">", FL_AT);

        return stack.top();
    }

    void Function::main() {
        Function f;
        std::string notation = "3+4*2/(1-5)^2^3";
        FL_LOG(f.toPostfix(notation));
        FL_LOG("P: " << f.parse(notation)->toInfix());
        FL_LOG(f.parse(notation)->evaluate());
        //3 4 2 * 1 5 - 2 3 ^ ^ / +

        f.variables["y"] = 1.0;
        notation = "sin(y*x)^2/x";
        FL_LOG("pre: " << f.parse(notation)->toPrefix());
        FL_LOG("in: " << f.parse(notation)->toInfix());
        FL_LOG("pos: " << f.parse(notation)->toPostfix());
        f.load(notation);
        FL_LOG("Result: " << f.membership(1));
        //y x * sin 2 ^ x /

        notation = "(Temperature is High and Oxigen is Low) or "
                "(Temperature is Low and (Oxigen is Low or Oxigen is High))";
        FL_LOG(f.toPostfix(notation));

        f.variables["pi"] = 3.14;
        notation = "-5 *4/sin(-pi/2)";
        FL_LOG(f.toPostfix(notation));
        try {
            FL_LOG(f.parse(notation)->evaluate());
        } catch (fl::Exception& e) {
            FL_LOG(e.getWhat());
        }
        f.variables["pi"] = 3.14;
        notation = "~5 *4/sin(~pi/2)";
        FL_LOG(f.toPostfix(notation));
        try {
            FL_LOG(f.parse(notation)->evaluate(&f.variables));
        } catch (fl::Exception& e) {
            FL_LOG(e.getWhat());
        }
    }


}