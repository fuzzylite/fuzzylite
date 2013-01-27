/* 
 * File:   TermFactory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:18 PM
 */

#ifndef FL_TERMFACTORY_H
#define	FL_TERMFACTORY_H


#include "fl/scalar.h"

#include <vector>
#include <string>

namespace fl {
    class Term;

    class FL_EXPORT TermFactory {
    public:
        TermFactory();
        virtual ~TermFactory();

        virtual Term* create(const std::string& className,
                const std::vector<scalar>& params) const;

        virtual std::vector<std::string> available() const;

    };
}
#endif	/* FL_TERMFACTORY_H */

