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
 * File:   Factory.cpp
 * Author: jcrada
 *
 * Created on 29 November 2013, 11:48 PM
 */

#include "fl/factory/Factory.h"
#include "fl/Exception.h"

#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/hedge/Hedge.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"
#include "fl/term/Term.h"


namespace fl {

    template <typename T>
    Factory<T>::Factory() {

    }

    template <typename T>
    Factory<T>::~Factory() {

    }

    template <typename T>
    void Factory<T>::registerClass(const std::string& key, Creator creator) {
        this->map[key] = creator;
    }

    template <typename T>
    void Factory<T>::deregisterClass(const std::string& key) {
        typename std::map<std::string, Creator>::iterator it = this->map.find(key);
        if (it != this->map.end()) {
            this->map.erase(it);
        }
    }

    template <typename T>
    bool Factory<T>::hasRegisteredClass(const std::string& key) const {
        typename std::map<std::string, Creator>::const_iterator it = this->map.find(key);
        return (it != this->map.end());
    }

    template <typename T>
    std::vector<std::string> Factory<T>::available() const {
        std::vector<std::string> result;
        typename std::map<std::string, Creator>::const_iterator it = this->map.begin();
        while (it != this->map.end()) {
            result.push_back(it->first);
            ++it;
        }
        return result;
    }

    template <typename T>
    T Factory<T>::createInstance(const std::string& key) const {
        if (key.empty()) return NULL;
        typename std::map<std::string, Creator>::const_iterator it = this->map.find(key);
        if (it != this->map.end()) {
            if (it->second) {
                return it->second();
            }
            return NULL;
        }
        std::ostringstream ss;
        ss << "[factory error] class <" << key << "> not registered";
        throw fl::Exception(ss.str(), FL_AT);
    }
}

template class fl::Factory<fl::Defuzzifier*>;
template class fl::Factory<fl::Hedge*>;
template class fl::Factory<fl::SNorm*>;
template class fl::Factory<fl::TNorm*>;
template class fl::Factory<fl::Term*>;


