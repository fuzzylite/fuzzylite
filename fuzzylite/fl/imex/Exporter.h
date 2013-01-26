/* 
 * File:   Exporter.h
 * Author: jcrada
 *
 * Created on 25 December 2012, 11:40 PM
 */

#ifndef FL_EXPORTER_H
#define	FL_EXPORTER_H

#include <string>
#include "fl/config.h"
namespace fl {
    class Engine;
    flclass Exporter{
    public:
        Exporter(){}
        virtual ~Exporter(){}
        
        virtual std::string name() const = 0;
        virtual std::string toString(const Engine* engine) const = 0;
        
    };
    
}

#endif	/* FL_EXPORTER_H */

