/*   Copyright 2013 Juan Rada-Vilela

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/* 
 * File:   Exporter.h
 * Author: jcrada
 *
 * Created on 25 December 2012, 11:40 PM
 */

#ifndef FL_EXPORTER_H
#define	FL_EXPORTER_H

#include "fl/fuzzylite.h"

#include <string>

namespace fl {
    class Engine;
    class FL_EXPORT Exporter{
    public:
        Exporter(){}
        virtual ~Exporter(){}
        
        virtual std::string name() const = 0;
        virtual std::string toString(const Engine* engine) const = 0;
        
    };
    
}

#endif	/* FL_EXPORTER_H */

