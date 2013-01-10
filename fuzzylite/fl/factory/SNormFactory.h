/* 
 * File:   SNormFactory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:20 PM
 */

#ifndef FL_SNORMFACTORY_H
#define	FL_SNORMFACTORY_H

#include <string>

namespace fl {
    class SNorm;
    
    class SNormFactory {
    public:
        SNormFactory();
        virtual ~SNormFactory();
        
        virtual SNorm* create(const std::string& className) const;
    };
}
#endif	/* FL_SNORMFACTORY_H */

