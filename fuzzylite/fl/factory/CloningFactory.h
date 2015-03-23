/*
 Author: Juan Rada-Vilela, Ph.D.
 Copyright (C) 2010-2014 FuzzyLite Limited
 All rights reserved

 This file is part of fuzzylite.

 fuzzylite is free software: you can redistribute it and/or modify it under
 the terms of the GNU Lesser General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 fuzzylite is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with fuzzylite.  If not, see <http://www.gnu.org/licenses/>.

 fuzzylite™ is a trademark of FuzzyLite Limited.

 */

#ifndef FL_CLONINGFACTORY_H
#define FL_CLONINGFACTORY_H

#include "fl/Exception.h"
#include "fl/fuzzylite.h"
#include "fl/term/Function.h"

#include <map>
#include <string>
#include <vector>

namespace fl {

    template <typename T>
    class FL_API CloningFactory {
    protected:
        std::string _name;
        std::map<std::string, T> _objects;

    public:
        explicit CloningFactory(const std::string& name = "")
        : _name(name)
        {
        }

        CloningFactory(const CloningFactory& other)
        : _name(other._name)
        {
            typename std::map<std::string, T>::const_iterator it = other._objects.begin();
            while (it != other._objects.end()) {
                T clone = fl::null;
                if (it->second) clone = it->second->clone();
                this->_objects[it->first] = clone;
                ++it;
            }
        }

        CloningFactory& operator=(const CloningFactory& other) {
            if (this != &other) {
                _name = other._name;

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

        virtual ~CloningFactory() {
            typename std::map<std::string, T>::const_iterator it = this->_objects.begin();
            while (it != this->_objects.end()) {
                if (it->second) delete it->second;
                ++it;
            }
        }

        FL_DEFAULT_MOVE(CloningFactory)

        virtual std::string name() const {
            return _name;
        }

        virtual void registerObject(const std::string& key, T object) {
            _objects[key] = object;
        }

        virtual void deregisterObject(const std::string& key) {
            typename std::map<std::string, T>::iterator it = this->_objects.find(key);
            if (it != this->_objects.end()) {
                this->_objects.erase(it);
                delete it->second;
            }
        }

        virtual bool hasObject(const std::string& key) const {
            typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
            return (it != this->_objects.end());
        }

        virtual T getObject(const std::string& key) const {
            typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
            if (it != this->_objects.end()) {
                if (it->second) return it->second;
            }
            return fl::null;
        }

        virtual T cloneObject(const std::string& key) const {
            typename std::map<std::string, T>::const_iterator it = this->_objects.find(key);
            if (it != this->_objects.end()) {
                if (it->second) return it->second->clone();
                return fl::null;
            }
            throw fl::Exception("[cloning error] " + _name + " object by name <" + key + "> not registered", FL_AT);
        }

        virtual std::vector<std::string> available() const {
            std::vector<std::string> result;
            typename std::map<std::string, T>::const_iterator it = this->_objects.begin();
            while (it != this->_objects.end()) {
                result.push_back(it->first);
            }
            return result;
        }
    };
}

#endif  /* FL_CLONINGFACTORY_H */

