/* 
 * File:   DefuzzifierFactory.h
 * Author: jcrada
 *
 * Created on 8 January 2013, 11:17 PM
 */

#ifndef FL_DEFUZZIFIERFACTORY_H
#define	FL_DEFUZZIFIERFACTORY_H

#include "fl/config.h"

#include <string>
#include <vector>

namespace fl {
    class Defuzzifier;
    
    class FL_EXPORT DefuzzifierFactory {
    public:
        DefuzzifierFactory();
        virtual ~DefuzzifierFactory();

        virtual Defuzzifier* create(const std::string& className,
                int divisions = FL_DIVISIONS);
        
        virtual std::vector<std::string> available() const;
        
    };
}
#endif	/* DEFUZZIFIERFACTORY_H */

