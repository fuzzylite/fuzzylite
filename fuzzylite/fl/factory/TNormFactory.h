/* 
 * File:   TNormFactory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:20 PM
 */

#ifndef FL_TNORMFACTORY_H
#define	FL_TNORMFACTORY_H

#include <string>
#include <vector>

namespace fl {
    class TNorm;

    class TNormFactory {
    public:
        TNormFactory();
        virtual ~TNormFactory();

        virtual TNorm* create(const std::string& className) const;
        virtual std::vector<std::string> available() const;
    };
}
#endif	/* FL_TNORMFACTORY_H */

