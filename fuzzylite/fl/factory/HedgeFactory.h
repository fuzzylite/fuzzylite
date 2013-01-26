/* 
 * File:   HedgeFactory.h
 * Author: jcrada
 *
 * Created on 11 January 2013, 8:51 PM
 */

#ifndef FL_HEDGEFACTORY_H
#define	FL_HEDGEFACTORY_H

#include <string>
#include <vector>
#include "fl/config.h"
namespace fl {
    class Hedge;
    flclass HedgeFactory {
    public:
        HedgeFactory();
        virtual ~HedgeFactory();
        
        virtual Hedge* create(const std::string& name) const;
        virtual std::vector<std::string> available() const;
    };
}
#endif	/* FL_HEDGEFACTORY_H */

