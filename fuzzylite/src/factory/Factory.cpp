// #BEGIN_LICENSE
// fuzzylite: a fuzzy logic control library in C++
// Copyright (C) 2014  Juan Rada-Vilela
// 
// This file is part of fuzzylite.
//
// fuzzylite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// fuzzylite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.
// #END_LICENSE

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
    void Factory<T>::registerClass(const std::string& key, Constructor creator) {
        this->map[key] = creator;
    }

    template <typename T>
    void Factory<T>::deregisterClass(const std::string& key) {
        typename std::map<std::string, Constructor>::iterator it = this->map.find(key);
        if (it != this->map.end()) {
            this->map.erase(it);
        }
    }

    template <typename T>
    bool Factory<T>::hasRegisteredClass(const std::string& key) const {
        typename std::map<std::string, Constructor>::const_iterator it = this->map.find(key);
        return (it != this->map.end());
    }

    template <typename T>
    std::vector<std::string> Factory<T>::available() const {
        std::vector<std::string> result;
        typename std::map<std::string, Constructor>::const_iterator it = this->map.begin();
        while (it != this->map.end()) {
            result.push_back(it->first);
            ++it;
        }
        return result;
    }

    template <typename T>
    T Factory<T>::createInstance(const std::string& key) const {
        if (key.empty()) return NULL;
        typename std::map<std::string, Constructor>::const_iterator it = this->map.find(key);
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


