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
 * File:   Factory.h
 * Author: jcrada
 *
 * Created on 29 November 2013, 7:15 AM
 */

#ifndef FL_FACTORY_H
#define	FL_FACTORY_H

#include "fl/fuzzylite.h"

#include <map>
#include <string>
#include <vector>

namespace fl {

    template <typename T>
    class FL_EXPORT Factory {
    public:
        typedef T(*Creator)();

    protected:
        std::map<std::string, Creator> map;

    public:
        Factory();
        virtual ~Factory();

        virtual void registerClass(const std::string& key, Creator creator);
        virtual void deregisterClass(const std::string& key);
        virtual bool hasRegisteredClass(const std::string& key) const;
        virtual std::vector<std::string> available() const;
        virtual T createInstance(const std::string& key) const;
    };
    
}

#endif	/* FL_FACTORY_H */

