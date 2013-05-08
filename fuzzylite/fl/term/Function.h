/* 
 * File:   Function.h
 * Author: jcrada
 *
 * Created on 30 March 2013, 1:13 PM
 */

#ifndef FL_FUNCTION_H
#define	FL_FUNCTION_H

#include "fl/term/Term.h"


namespace fl {

    class FL_EXPORT Function : public Term {
    public:

        //        struct FL_EXPORT Element {
        //            std::string name;
        //            Element(const std::string& name);
        //            virtual scalar evaluate(const std::map<std::string, scalar>* variables) const;
        //            virtual std::string toString() const;
        //        };
        //
        //        struct FL_EXPORT Operator : public Element {
        //            short precedence;
        //            short arity;
        //            short associativity;
        //            Operator(const std::string& name, short precedence, short arity = 2,
        //                    short associativity = -1);
        //
        //            std::string toString() const;
        //        };
        //
        //        typedef double(*OneArgFunction)(double);
        //        typedef double(*TwoArgFunction)(double, double);
        //
        //        struct FL_EXPORT BuiltInFunction : public Element {
        //            std::string name;
        //            short arity;
        //            short associativity;
        //            OneArgFunction oneArgFunction;
        //            TwoArgFunction twoArgFunction;
        //
        //            BuiltInFunction(const std::string& name, short arity = 1, short associativity = -1);
        //        };
    protected:
        //        Element* _root;

        Function(const std::string& name = "",
                const std::string& infixFunction = "");
        virtual ~Function();

        virtual std::string className() const;
        virtual Function* copy() const;

        virtual scalar membership(scalar x) const;
        virtual std::string toString() const;

        virtual void setInfixFunction(const std::string& infixFunction);
        virtual std::string getInfixFunction() const;
    };

}

#endif	/* FL_FUNCTION_H */

