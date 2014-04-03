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
 * File:   ConstructorFactory.cpp
 * Author: jcrada
 *
 * Created on 29 November 2013, 11:48 PM
 */

#include "fl/factory/ConstructionFactory.h"
#include "fl/Exception.h"

#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/hedge/Hedge.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"
#include "fl/term/Term.h"
#include "fl/term/Function.h"


namespace fl {

    template <typename T>
    ConstructionFactory<T>::ConstructionFactory() {

    }

    template <typename T>
    ConstructionFactory<T>::~ConstructionFactory() {
    }

    template <typename T>
    void ConstructionFactory<T>::registerConstructor(const std::string& key, Constructor constructor) {
        this->_constructors[key] = constructor;
    }

    template <typename T>
    void ConstructionFactory<T>::deregisterConstructor(const std::string& key) {
        typename std::map<std::string, Constructor>::iterator it = this->_constructors.find(key);
        if (it != this->_constructors.end()) {
            this->_constructors.erase(it);
        }
    }

    template <typename T>
    bool ConstructionFactory<T>::hasConstructor(const std::string& key) const {
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
        return (it != this->_constructors.end());
    }

    template <typename T>
    typename ConstructionFactory<T>::Constructor ConstructionFactory<T>::getConstructor(const std::string& key) const {
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
        if (it != this->_constructors.end()) {
            return it->second;
        }
        return NULL;
    }

    template <typename T>
    T ConstructionFactory<T>::constructObject(const std::string& key) const {
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
        if (it != this->_constructors.end()) {
            if (it->second) {
                return it->second();
            }
            return NULL;
        }
        std::ostringstream ss;
        ss << "[factory error] constructor <" << key << "> not registered";
        throw fl::Exception(ss.str(), FL_AT);
    }

    template <typename T>
    std::vector<std::string> ConstructionFactory<T>::available() const {
        std::vector<std::string> result;
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.begin();
        while (it != this->_constructors.end()) {
            result.push_back(it->first);
            ++it;
        }
        return result;
    }

    template class ConstructionFactory<Defuzzifier*>;
    template class ConstructionFactory<Hedge*>;
    template class ConstructionFactory<SNorm*>;
    template class ConstructionFactory<TNorm*>;
    template class ConstructionFactory<Term*>;
}



