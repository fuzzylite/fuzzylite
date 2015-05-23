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

#ifndef FL_CLONINGFACTORY_H
#define FL_CLONINGFACTORY_H

#include "fl/fuzzylite.h"

#include <map>
#include <string>
#include <vector>

namespace fl {

    template <typename T>
    class CloningFactory {
    protected:
        std::string _name;
        std::map<std::string, T> _objects;

    public:
        explicit CloningFactory(const std::string& name = "");
        CloningFactory(const CloningFactory& other);
        CloningFactory& operator=(const CloningFactory& other);
        virtual ~CloningFactory();
        FL_DEFAULT_MOVE(CloningFactory)

        virtual std::string name() const;

        virtual void registerObject(const std::string& key, T object);
        virtual void deregisterObject(const std::string& key);
        virtual bool hasObject(const std::string& key) const;
        virtual T getObject(const std::string& key) const;
        virtual T cloneObject(const std::string& key) const;
        virtual std::vector<std::string> available() const;

    };
}

/**
 * Template implementation
 */

#include "fl/Exception.h"

namespace fl {

    template<typename T>
    inline CloningFactory<T>::CloningFactory(const std::string& name) : _name(name) {

    }

    template<typename T>
    inline CloningFactory<T>::CloningFactory(const CloningFactory& other) {
        typename std::map<std::string, T>::const_iterator it = other._objects.begin();
        while (it != other._objects.end()) {
            T clone = fl::null;
            if (it->second) clone = it->second->clone();
            this->_objects[it->first] = clone;
            ++it;
        }
    }

    template<typename T>
    inline CloningFactory<T>& CloningFactory<T>::operator=(const CloningFactory& other) {
        if (this != &other) {
            typename std::map<std::string, T>::const_iterator it = this->_objects.begin();
            while (it != this->_objects.end()) {
                if (it->second) delete it->second;
                ++it;
            }
            this->_objects.clear();

            it = other._objects.begin();
            while (it != other._objects.end()) {
                T clone = fl::null;
                if (it->second) clone = it->second->clone();
                this->_objects[it->first] = clone;
                ++it;
            }
        }
        return *this;
    }

    template<typename T>
    inline CloningFactory<T>::~CloningFactory() {
        typename std::map<std::string, T>::const_iterator it = this->_objects.begin();
        while (it != this->_objects.end()) {
            if (it->second) delete it->second;
            ++it;
        }
    }

    template<typename T>
    inline std::string CloningFactory<T>::name() const {
        return this->_name;
    }

    template<typename T>
    inline void CloningFactory<T>::registerObject(const std::string& key, T object) {
        this->_objects[key] = object;
    }

    template<typename T>
    inline void CloningFactory<T>::deregisterObject(const std::string& key) {
        typename std::map<std::string, T>::iterator it = this->_objects.find(key);
        if (it != this->_objects.end()) {
            this->_objects.erase(it);
            delete it->second;
        }
    }

    template<typename T>
    inline bool CloningFactory<T>::hasObject(const std::string& key) const {
        typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
        return (it != this->_objects.end());
    }

    template<typename T>
    inline T CloningFactory<T>::getObject(const std::string& key) const {
        typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
        if (it != this->_objects.end()) {
            if (it->second) return it->second;
        }
        return fl::null;
    }

    template<typename T>
    inline T CloningFactory<T>::cloneObject(const std::string& key) const {
        typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
        if (it != this->_objects.end()) {
            if (it->second) return it->second->clone();
            return fl::null;
        }
        throw fl::Exception("[cloning error] " + _name + " object by name <" + key + "> not registered", FL_AT);
    }

    template<typename T>
    inline std::vector<std::string> CloningFactory<T>::available() const {
        std::vector<std::string> result;
        typename std::map<std::string, T>::const_iterator it = this->_objects.begin();
        while (it != this->_objects.end()) {
            result.push_back(it->first);
        }
        return result;
    }
}

#endif  /* FL_CLONINGFACTORY_H */

