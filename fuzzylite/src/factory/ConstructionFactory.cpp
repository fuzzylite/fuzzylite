/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright © 2010-2015 FuzzyLite Limited.
 All rights reserved.

 This file is part of fuzzylite®.

 fuzzylite® is free software: you can redistribute it and/or modify it under
 the terms of the FuzzyLite License included with the software.

 You should have received a copy of the FuzzyLite License along with 
 fuzzylite®. If not, see <http://www.fuzzylite.com/license/>.

 fuzzylite® is a registered trademark of FuzzyLite Limited.

 */

#include "fl/factory/ConstructionFactory.h"

#include "fl/Exception.h"
#include "fl/defuzzifier/Defuzzifier.h"
#include "fl/hedge/Hedge.h"
#include "fl/norm/SNorm.h"
#include "fl/norm/TNorm.h"
#include "fl/term/Function.h"
#include "fl/term/Term.h"



namespace fl {

    template <typename T>
    ConstructionFactory<T>::ConstructionFactory(const std::string& name) : _name(name) {

    }

    template <typename T>
    ConstructionFactory<T>::~ConstructionFactory() {
    }

    template<typename T>
    std::string ConstructionFactory<T>::name() const {
        return this->_name;
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
        return fl::null;
    }

    template <typename T>
    T ConstructionFactory<T>::constructObject(const std::string& key) const {
        typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
        if (it != this->_constructors.end()) {
            if (it->second) {
                return it->second();
            }
            return fl::null;
        }
        std::ostringstream ss;
        ss << "[factory error] constructor of " + _name + " <" << key << "> not registered";
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



