/* 
 * File:   Function.cpp
 * Author: jcrada
 *
 * Created on 7 April 2013, 10:17 PM
 */

#include "fl/term/Function.h"

#include "fl/rule/Rule.h"

#include <functional>
#include <queue>
#include <stack>
namespace fl {

    /**
     * Parsing elements
     */

    Function::Element::Element(const std::string& name)
    : name(name) { }

    template <typename T>
    Function::Operator<T>::Operator(const std::string& name, T functionPointer,
            short precedence, short arity, short associativity)
    : Element(name), functionPointer(functionPointer),
    precedence(precedence), arity(arity), associativity(associativity) { }

    template Function::Operator<Function::Unary>::Operator(const std::string& name,
            Function::Unary functionPointer,
            short precedence, short arity, short associativity);
    template Function::Operator<Function::Binary>::Operator(const std::string& name,
            Function::Binary functionPointer,
            short precedence, short arity, short associativity);
    //    template Function::Operator<Function::Tertiary>::Operator(const std::string& name,
    //            Function::Tertiary functionPointer,
    //            short precedence, short arity, short associativity);

    template <typename T>
    std::string Function::Operator<T>::toString() const {
        std::ostringstream ss;
        ss << "Operator (name=" << name << ", "
                << "precedence=" << precedence << ", "
                << "arity=" << arity << ", "
                << "associativity=" << associativity << ", "
                << "pointer=" << functionPointer
                << ")";
        return ss.str();
    }

    template <typename T>
    Function::BuiltInFunction<T>::BuiltInFunction(const std::string& name,
            T functionPointer, short arity, short associativity)
    : Element(name), functionPointer(functionPointer),
    arity(arity), associativity(associativity) { }

    template Function::BuiltInFunction<Function::Unary>::BuiltInFunction(const std::string& name,
            Function::Unary functionPointer, short arity, short associativity);
    template Function::BuiltInFunction<Function::Binary>::BuiltInFunction(const std::string& name,
            Function::Binary functionPointer, short arity, short associativity);
    //    template Function::BuiltInFunction<Function::Tertiary>::BuiltInFunction(const std::string& name,
    //            Function::Tertiary functionPointer, short arity, short associativity);

    template <typename T>
    std::string Function::BuiltInFunction<T>::toString() const {
        std::ostringstream ss;
        ss << "BuiltInFunction (name=" << name << ", "
                << "arity=" << arity << ", "
                << "associativity=" << associativity << ", "
                << "pointer=" << functionPointer
                << ")";
        return ss.str();
    }

    /***********************************
     * Load Operators and Functions
     ***********************************/

    void Function::loadOperators() {
        char p = 7;
        // (!) Logical and (~) Bitwise NOT
        //        this->_unaryOperators["!"] = new Operator<Unary>("!", std::logical_not<scalar>, p, 1);
        //        this->_unaryOperators["~"] = new Operator<Unary>("~", Function::complement, 1);
        --p; //Power
        this->_binaryOperators["^"] = new Operator<Binary>("^", std::pow, p, 2, 1);
        --p; //Multiplication, Division, and Modulo
        this->_binaryOperators["*"] = new Operator<Binary>("*", fl::Op::multiplies, p);
        this->_binaryOperators["/"] = new Operator<Binary>("/", fl::Op::divides, p);
        this->_binaryOperators["%"] = new Operator<Binary>("%", fl::Op::modulus, p);
        --p; //Addition, Subtraction
        this->_binaryOperators["+"] = new Operator<Binary>("+", fl::Op::plus, p);
        this->_binaryOperators["-"] = new Operator<Binary>("-", fl::Op::minus, p);
        //        --p; //Bitwise AND
        //        this->_binaryOperators["&"] = new Operator<Binary>("&", std::bit_and, p);
        //        --p; //Bitwise OR
        //        this->_binaryOperators["|"] = new Operator<Binary>("|", std::bit_or, p);
        //        --p; //Logical AND
        //        this->_binaryOperators["&&"] = new Operator<Binary>("&&", std::logical_and, p);
        //        --p; //Logical OR
        //        this->_binaryOperators["||"] = new Operator<Binary>("||", std::logical_or, p);
    }

    void Function::loadBuiltInFunctions() {
        this->_unaryFunctions["acos"] = new BuiltInFunction<Unary>("acos", &(std::acos));
        this->_unaryFunctions["asin"] = new BuiltInFunction<Unary>("asin", &(std::asin));
        this->_unaryFunctions["atan"] = new BuiltInFunction<Unary>("atan", &(std::atan));

        this->_unaryFunctions["ceil"] = new BuiltInFunction<Unary>("ceil", &(std::ceil));
        this->_unaryFunctions["cos"] = new BuiltInFunction<Unary>("cos", &(std::cos));
        this->_unaryFunctions["cosh"] = new BuiltInFunction<Unary>("cosh", &(std::cosh));
        this->_unaryFunctions["exp"] = new BuiltInFunction<Unary>("exp", &(std::exp));
        this->_unaryFunctions["fabs"] = new BuiltInFunction<Unary>("fabs", &(std::fabs));
        this->_unaryFunctions["floor"] = new BuiltInFunction<Unary>("floor", &(std::floor));
        this->_unaryFunctions["log"] = new BuiltInFunction<Unary>("log", &(std::log));
        this->_unaryFunctions["log10"] = new BuiltInFunction<Unary>("log10", &(std::log10));

        this->_unaryFunctions["sin"] = new BuiltInFunction<Unary>("sin", &(std::sin));
        this->_unaryFunctions["sinh"] = new BuiltInFunction<Unary>("sinh", &(std::sinh));
        this->_unaryFunctions["sqrt"] = new BuiltInFunction<Unary>("sqrt", &(std::sqrt));
        this->_unaryFunctions["tan"] = new BuiltInFunction<Unary>("tan", &(std::tan));
        this->_unaryFunctions["tanh"] = new BuiltInFunction<Unary>("tanh", &(std::tanh));

#ifdef FL_UNIX
        //not found in Windows
        this->_unaryFunctions["log1p"] = new BuiltInFunction<Unary>("log1p", &(log1p));
        this->_unaryFunctions["acosh"] = new BuiltInFunction<Unary>("acosh", &(acosh));
        this->_unaryFunctions["asinh"] = new BuiltInFunction<Unary>("asinh", &(asinh));
        this->_unaryFunctions["atanh"] = new BuiltInFunction<Unary>("atanh", &(atanh));
#endif

        this->_binaryFunctions["pow"] = new BuiltInFunction<Binary>("pow", &(std::pow));
        this->_binaryFunctions["atan2"] = new BuiltInFunction<Binary>("atan2", &(std::atan2));
        this->_binaryFunctions["fmod"] = new BuiltInFunction<Binary>("fmod", &(std::fmod));
    }

    /**********************************
     * Function class.
     **********************************/
    Function::Function(const std::string& name, bool loadBuiltInFunctions)
    : Term(name) {
        loadOperators();
        if (loadBuiltInFunctions) {
            this->loadBuiltInFunctions();
        }
    }

    Function::~Function() { }

    Function* Function::create(const std::string& name,
            const std::string& infix, const Engine* engine) throw (fl::Exception) {
        Function* result = new Function(name);
        result->load(infix, engine);
        return result;
    }

    scalar Function::membership(scalar x) const {
        (void) x;
        //        std::map<std::string, scalar> mapping;
        //TODO:load 
        return 0;
    }

    std::string Function::className() const {
        return "Function";
    }

    std::string Function::toString() const {
        return "Function (" + _infix + ")";
    }

    Function* Function::copy() const {
        //TODO: Not sure this is correct. Deep clone?
        return new Function(*this);
    }

    void Function::load(const std::string& infix,
            const Engine* engine) throw (fl::Exception) {
        this->_infix = infix;
        this->_engine = engine;
        std::string postfix = toPostfix(infix);
        (void) postfix;
    }

    std::string Function::getInfix() const {
        return this->_infix;
    }

    const Engine* Function::getEngine() const {
        return this->_engine;
    }

    /**
     * Infix to Postfix
     */

    std::string Function::toPostfix(const std::string& rawInfix) const {
        std::string infix = rawInfix;

        //TODO: inserts spaces in all operators, parentheses, and commas.
        std::vector<std::string> space;
        space.push_back("(");
        space.push_back(")");
        space.push_back(",");
        {
            std::map<std::string, Operator<Binary>*>::const_iterator itOp = this->_binaryOperators.begin();
            for (; itOp != this->_binaryOperators.end(); ++itOp) {
                space.push_back(itOp->first);
            }
        }

        for (std::size_t i = 0; i < space.size(); ++i) {
            infix = fl::Op::findReplace(infix, space[i], " " + space[i] + " ", true);
        }
        FL_DBG("infix=" << infix);

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
                    ex << "mismatching parentheses in: " << rawInfix;
                    throw fl::Exception(ex.str(), FL_AT);
                }

            } else if (isOperator(token)) {
                Operator<Binary>* op1 = getOperator(token);
                while (true) {
                    Operator<Binary>* op2 = NULL;
                    if (not stack.empty() and isOperator(stack.top())) {
                        op2 = this->getOperator(stack.top());
                    } else
                        break;

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
                    ex << "mismatching parentheses in: " << rawInfix;
                    throw fl::Exception(ex.str(), FL_AT);
                }
                stack.pop(); //get rid of "("

                if (not stack.empty() and isBuiltInFunction(stack.top())) {
                    queue.push(stack.top());
                    stack.pop();
                }
            } else {
                std::ostringstream ex;
                ex << "this should have never occurred!";
                throw fl::Exception(ex.str(), FL_AT);
            }
        }

        while (not stack.empty()) {
            if (stack.top() == "(" or stack.top() == ")") {
                std::ostringstream ex;
                ex << "mismatching parentheses in: " << rawInfix;
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
        FL_DBG("postfix=" << ssPostfix.str());
        return ssPostfix.str();
    }

    Function::Operator<Function::Binary>*
    Function::getOperator(const std::string& key) const {
        std::map<std::string, Operator<Binary>*>::const_iterator it =
                this->_binaryOperators.find(key);
        if (it == this->_binaryOperators.end()) return NULL;
        return it->second;
    }

    template<> Function::BuiltInFunction<Function::Unary>*
    Function::getBuiltInFunction(const std::string& key) const {
        std::map<std::string, Function::BuiltInFunction<Function::Unary>*>::const_iterator it =
                this->_unaryFunctions.find(key);
        if (it == this->_unaryFunctions.end()) return NULL;
        return it->second;
    }

    template<> Function::BuiltInFunction<Function::Binary>*
    Function::getBuiltInFunction(const std::string& key) const {
        std::map<std::string, Function::BuiltInFunction<Function::Binary>*>::const_iterator it =
                this->_binaryFunctions.find(key);
        if (it == this->_binaryFunctions.end()) return NULL;
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
        return name == fl::Rule::andKeyword() || name == fl::Rule::orKeyword() ||
                _binaryOperators.find(name) != _binaryOperators.end();
    }

    bool Function::isBuiltInFunction(const std::string& name) const {
        return _unaryFunctions.find(name) != _unaryFunctions.end() or
                _binaryFunctions.find(name) != _binaryFunctions.end();
    }

    void Function::main() {
        Function f;
        std::string notation = "3+4*2/(1-5)^2^3";
        FL_LOG(f.toPostfix(notation));
        //3 4 2 * 1 5 - 2 3 ^ ^ / +

        notation = "3+4*2/(1-5)^2^3";
        FL_LOG(f.toPostfix(notation));
        //3 4 2 * 1 5 - 2 3 ^ ^ / +

        notation = "sin(y*x)^2/x";
        FL_LOG(f.toPostfix(notation));
        //y x * sin 2 ^ x /

        notation = "(Temperature is High and Oxigen is Low) or "
                "(Temperature is Low and (Oxigen is Low or Oxigen is High))";
        FL_LOG(f.toPostfix(notation));
    }


}