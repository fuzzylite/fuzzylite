/* 
 * File:   Exporter.h
 * Author: jcrada
 *
 * Created on 25 December 2012, 11:40 PM
 */

#ifndef FL_EXPORTER_H
#define	FL_EXPORTER_H

#include <string>

namespace fl {
    class Engine;
    class Exporter{
    public:
        Exporter(){}
        virtual ~Exporter(){}
        
        virtual std::string name() const = 0;
        virtual std::string toString(const Engine* engine) = 0;
        
    };
    
}

#endif	/* FL_EXPORTER_H */

