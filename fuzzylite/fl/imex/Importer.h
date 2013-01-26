/* 
 * File:   Importer.h
 * Author: jcrada
 *
 * Created on 25 December 2012, 11:43 PM
 */

#ifndef FL_IMPORTER_H
#define	FL_IMPORTER_H

#include "fl/config.h"

#include <string>

namespace fl {
    class Engine;

    flclass Importer {
    public:

        Importer() {
        }

        virtual ~Importer() {
        }
        
        virtual std::string name() const = 0;
        virtual Engine* fromString(const std::string& s) const = 0;
    };

}

#endif	/* IMPORTER_H */

