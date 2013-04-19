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
        Function(const std::string& name = "", 
                const std::string& infixFunction = "");
        ~Function();
        
        std::string className() const;
        Function* copy() const;

        scalar membership(scalar x) const;
        std::string toString() const;

        void setInfixFunction(const std::string& infixFunction );
        std::string getInfixFunction() const;
    };

}

#endif	/* FL_FUNCTION_H */

