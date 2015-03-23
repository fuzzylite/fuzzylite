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

#ifndef FL_FACTORY_H
#define FL_FACTORY_H

#include "fl/Exception.h"
#include "fl/fuzzylite.h"

#include <map>
#include <string>
#include <vector>

namespace fl {

    template <typename T>
    class FL_API ConstructionFactory {
    public:
        typedef T(*Constructor)();

    protected:
        std::string _name;
        std::map<std::string, Constructor> _constructors;

    public:
        explicit ConstructionFactory(const std::string& name)
        : _name(name)
        {
        }

        virtual ~ConstructionFactory() {
        }

        FL_DEFAULT_COPY_AND_MOVE(ConstructionFactory)

        virtual std::string name() const {
            return _name;
        }

        virtual void registerConstructor(const std::string& key, Constructor constructor) {
            _constructors[key] = constructor;
        }

        virtual void deregisterConstructor(const std::string& key) {
            typename std::map<std::string, Constructor>::iterator it = this->_constructors.find(key);
            if (it != this->_constructors.end()) {
                this->_constructors.erase(it);
            }
        }

        virtual bool hasConstructor(const std::string& key) const {
            typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
            return (it != this->_constructors.end());
        }

        virtual Constructor getConstructor(const std::string& key) const {
            typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.find(key);
            if (it != this->_constructors.end()) {
                return it->second;
            }
            return fl::null;
        }

        virtual T constructObject(const std::string& key) const {
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

        virtual std::vector<std::string> available() const {
            std::vector<std::string> result;
            typename std::map<std::string, Constructor>::const_iterator it = this->_constructors.begin();
            while (it != this->_constructors.end()) {
                result.push_back(it->first);
                ++it;
            }
            return result;
        }
    };

}

#endif  /* FL_FACTORY_H */

