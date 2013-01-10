/* 
 * File:   TermFactory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:18 PM
 */

#ifndef FL_TERMFACTORY_H
#define	FL_TERMFACTORY_H


#include "fl/scalar.h"

#include <map>
#include <string>

namespace fl {
    class Term;

    class TermFactory {
    public:
        TermFactory();
        virtual ~TermFactory();

        virtual Term* createTerm(const std::string& className,
                const std::map<std::string, scalar>& params) const;

    };
}
#endif	/* FL_TERMFACTORY_H */

